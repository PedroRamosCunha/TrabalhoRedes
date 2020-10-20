/*
Arqiuvo Sonda (Cliente).
O que faz:
-Coleta dados ambientais
-Envia os dados para a base (servidor)
*/

//Bibliotecas utilizadas
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Funcoes que geram dados falsos
float sensor_press(){	//Função que "coleta" os dados de pressao (em mmHh) que a sonda esta recebendo
	return (730.0+((float)(rand()%50)/20));
}

float sensor_temp(){	//Função que "coleta" os dados de temperatura (em °C) que a sonda esta recebendo
	return (0.0+((float)(50-rand()%100)/20));
}

float sensor_hum(){	//Função que "coleta" os dados de humidade relativa (em %) que a sonda esta recebendo
	return (33.3+((float)(50-rand()%100)/40));
}

float gps_lat(float last){	//Função que "coleta" os dados de latitude do GPS instalado na sonda
	last+=((float)(10-rand()%20)/100.0);

	if(fmod((double)last, 1.0)>=0.6){
		last+=0.4;
	}
	return last;
}

float gps_long(float last){	//Função que "coleta" os dados de longitude do GPS instalado na sonda
	last+=((float)(10-rand()%20)/100.0);

	if(fmod((double)last, 1.0)>=0.6){
		last+=0.4;
	}

	return last;
}

int main(){
	long int porta;	//porta de conexao com o servidor
	int ans_connect;	//resposta de conexao com o servidor
	int socket_servidor;
	struct sockaddr_in addr;
	float last_lat, last_long;
	int recebidos, enviados;
	char mensagem[64];
	char resposta[64];

	//Coordenadas de Sao Carlos
	last_lat =22.01;
	last_long = 47.53;

	//Seed do aleatorio
	srand(time(0)); 

	socket_servidor = socket(AF_INET,SOCK_STREAM,0);	//cria um socket de fluxo com protocolo TCP

	if(socket_servidor == -1){	//Condicao para caso a criacao do socket tenha falhado
		printf("Falha ao criar socket\n");
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);   //Porta de conexao inicial
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&(addr.sin_zero),0,sizeof(addr.sin_zero));

//Apos criar o socket, o cliente espera para receber o comunicado de que foi conectado ao servidor
	printf("Tentando se conectar a base...\n");

	if(connect(socket_servidor,(struct sockaddr*)&addr,sizeof(addr)) == -1){	//Condicao para caso a conexao com o servidor tenha falhado por algum motivo
		printf("Erro em connect()\n");
		return -2;
	}
	printf("Conectado\n\n");

	//Recebe porta da base
	recebidos = recv(socket_servidor,resposta,64,0);	//Recebe o numero da porta que ocorrera o envio dos dados da sonda
	resposta[recebidos] = '\0';

	porta = strtol(resposta, NULL, 10);		//Converte a string de resposta em um long int
	printf("Porta: %ld\n", porta);

	for(int i=0; i<500000000; i++){				//2 for para que o sistema não rode muito rapidamente e seja possível visualizar o que esta acontecendo
		for(int j; j<500000000; j++){}
	}

	close(socket_servidor);         
	socket_servidor = socket(AF_INET,SOCK_STREAM,0);	//Cria um novo protocolo para abrigar a nova interacao entre cliente e servidor com a porta que eles trocarão informações

	if(socket_servidor == -1){
		printf("Falha ao criar socket\n");
		return -1;
	}


	//Inicializacao da nova conexao com o servidor pela porta enviada por ele para transmissao dos dados
	//Conecta na porta fornecida pela base
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234+porta);   //Porta de conexao
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	memset(&(addr.sin_zero),0,sizeof(addr.sin_zero));

	//Apos criar o socket, o cliente espera para receber o comunicado de que foi conectado ao servidor
	printf("Tentando se conectar a base...\n");

	if(connect(socket_servidor,(struct sockaddr*)&addr,sizeof(addr)) == -1){	//Condicao para caso a conexao com o servidor tenha falhado por algum motivo
		printf("Erro em connect()\n");
		return -2;
	}
	printf("Conectado\n\n");
	

	do{
		//Transforma os dados em uma mensagem
		last_lat = gps_lat(last_lat);
		last_long = gps_long(last_long);
		sprintf(mensagem, "%f.2;%f.2;%f.2;%f.2;%f.2;", last_lat, last_long, sensor_press(), sensor_temp(), sensor_hum());

		//Envia os dados
		mensagem[63] = '\0';
		enviados = send(socket_servidor,mensagem,strlen(mensagem),0);

		//Aguarda resposta da base
		recebidos = recv(socket_servidor,resposta,64,0);
		resposta[recebidos] = '\0';
		printf("Base: %s\n",resposta);
	}while(enviados != -1); // Enquanto nao ha erro


	close(socket_servidor);         
	return 0;
}