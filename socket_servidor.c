/*
Arqiuvo Ground Station (servidor).
O que faz:
- recebe os dados (do cliente)
- exibe na tela
*/

//Bibliotecas utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

//Funcao para analisar as mensagens recebidas das sondas
void parse_dados(float *dados, char *mensagem){
	char buffer[16];
	int i, pos1, pos2;

	pos1 = 0;
	for(i=0; i<5; i++){		//Loop para separar a mensagem recebida dentro de uma matriz que receberá os dados separadamente para ser mostrado no terminal
		pos2=0;
		while(pos1 < 64 && mensagem[pos1]!=';' && pos2 < 15){
			buffer[pos2] = mensagem[pos1];
			pos1++;
			pos2++;
		}
		pos1++;
		buffer[pos2]='\0';
		dados[i] = strtof(buffer,NULL);		//Transferência da informação já separada presente no buffer para a variável dados, que será printada na main
	}
}

//Funcao que conecta a sonda ao ground station
void *conectar_sonda(void *arg){
	long int porta = *(long int *)arg;
	int meu_socket;
	int sock_cliente;
	struct sockaddr_in addr;

	long int count = 0;
	int recebidos, enviados; // Controle de bytes enviados e recebidos
	char mensagem[64];      // Buffer para envio de mensagens
	char resposta[64];     // Buffer para receber mensagens
	float dados[5];

	meu_socket = socket(AF_INET,SOCK_STREAM,0);		//cria um socket de fluxo com protocolo TCP

	if(meu_socket == -1){		//Condicao para caso a criacao do socket tenha falhado
		printf("Falha ao criar Socket\n");
		pthread_exit(NULL);
	}

	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(1234+porta);	//Cria uma porta que e igual a porta inicial de conexao+numero da nova porta para transmissao de dados
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

	if(bind(meu_socket,(struct sockaddr*)&addr,sizeof(addr)) == -1){	//Associa a porta criada ao sistema e analisa se ela ja nao esta sendo utilizada. Caso esteja, retorna um erro
		printf("Erro em bind()\n");
		pthread_exit(NULL);
	}

	if(listen(meu_socket,1) == -1){	//Habilita que o servidor se conecte com o cliente
		printf("Erro em listen()\n");
		pthread_exit(NULL);
	}

	printf("Aguardando cliente...\n");

	sock_cliente = accept(meu_socket,0,0);	//Funcao que aceita as conexoes com os clientes

	if(sock_cliente == -1){
		printf("Erro em accept()\n");
		pthread_exit(NULL);
	}

	printf("Recebendo dados da sonda %ld\n", porta);

	do{
		//Recebe os dados do cliente
		recebidos = recv(sock_cliente,resposta,64,0);
		resposta[recebidos] = '\0';

		//Faz o parse dos dados e os exibe
		parse_dados(dados, resposta);

		//Printa os dados recebidos
		printf("\nSonda %ld\n", porta);
		printf("Coordenadas: (%.2f,%.2f)\n",dados[0],dados[1]);
		printf("Pressao: %.2f\n",dados[2]);
		printf("Temperatura: %.2f\n",dados[3]);
		printf("Humidade: %.2f\n",dados[4]);


		for(int i=0; i<400000000; i++){	//2 for para que o sistema nao rode muito rapidamente e seja possivel visualizar o que está acontecendo
			for(int j; j<400000000; j++){}
		}

		//Envia a confirmação para a sonda
		count++;
		sprintf(mensagem, "%ld-OK", count);
		enviados = send(sock_cliente,mensagem,strlen(mensagem),0);

	}while(enviados != -1 ); // Enquanto nao ha erro
	
	//Apos acabar de receber as informações, o socket e a thread sao fechados
	close(sock_cliente);
	close(meu_socket);
	pthread_exit(NULL);
}

int main(){ 
	int    meu_socket;
	int    sock_cliente;
	struct sockaddr_in addr;
	long int count = 0;
	char mensagem[64];      // Buffer para envio de mensagens
	int enviados;          // Controle de bytes enviados

	pthread_t thread;		//Criacao de thread para conseguir receber dados simultaneos de varias sondas ao redor do mundo
	pthread_attr_t attr;

	meu_socket = socket(AF_INET,SOCK_STREAM,0);	//cria um socket de fluxo com protocolo TCP

	if(meu_socket == -1){		//Condicao para caso a criacao do socket tenha falhado
		printf("Falha ao criar Socket\n");
		return -1;
	}

	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(1234);	//Porta de conexao inicial com o cliente, para entao envia-lo para outra porta e receber os dados
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

	if(bind(meu_socket,(struct sockaddr*)&addr,sizeof(addr)) == -1){
		printf("Erro em bind()\n");
		return -2;
	}

	if(listen(meu_socket,1) == -1){
		printf("Erro em listen()\n");
		return -3;
	}

	do{		//While para aguardar cliente chamar o servidor e criar a conexao
		printf("Aguardando cliente...\n");

		sock_cliente = accept(meu_socket,0,0);

		if(sock_cliente == -1){
			printf("Erro em accept()\n");
			return -4;
		}

		count++;

		printf("Sonda %ld chegou\n", count);

		//Envia a confirmação para a sonda
		sprintf(mensagem, "%ld", count);
		enviados = send(sock_cliente,mensagem,strlen(mensagem),0);

		//Cria a thread de servidor para a sonda
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_create(&thread, &attr, conectar_sonda, &count);		//a thread e iniciada e entra na funcao que a sonda transmite os dados para o servidor

		close(sock_cliente);
	}while(enviados != -1 ); // Enquanto nao ha erro

	close(sock_cliente);
	close(meu_socket);
	return 0;
}