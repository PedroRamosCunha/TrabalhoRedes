#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>    //permite que use in_port_t e in_add_t do netinet

/*
-Sonda (Cliente)
-Coleta dados ambientais
-Envia para a base (servidor)
*/


//Funcoes que geram dados falsos
float sensor_press(){
	return (730.0+((float)(rand()%50)/20)); //mmHg
}
float sensor_temp(){
	return (0.0+((float)(50-rand()%100)/20)); // Celsius
}
float sensor_hum(){
	return (33.3+((float)(50-rand()%100)/40)); // Humidade Relativa %
}

float gps_lat(float last){
	last+=((float)(10-rand()%20)/100.0);

	if(fmod((double)last, 1.0)>=0.6){
		last+=0.4;
	}

	return last;
}
float gps_long(float last){
	last+=((float)(10-rand()%20)/100.0);

	if(fmod((double)last, 1.0)>=0.6){
		last+=0.4;
	}

	return last;
}


int main(){
	long int porta;
	int ans_connect;
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

	socket_servidor = socket(AF_INET,SOCK_STREAM,0);

	if(socket_servidor == -1){
		printf("Falha ao criar socket\n");
		return -1;
	}

	//Funções do <netinet.h>
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);   //Porta de conexao
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//fim

	memset(&(addr.sin_zero),0,sizeof(addr.sin_zero));

	printf("Tentando se conectar a base...\n");

	if(connect(socket_servidor,(struct sockaddr*)&addr,sizeof(addr)) == -1)  {
		printf("Erro em connect()\n");
		return -2;
	}
	printf("Conectado\n\n");

	//Recebe porta da base
	recebidos = recv(socket_servidor,resposta,64,0);
	resposta[recebidos] = '\0';

	porta = strtol(resposta, NULL, 10);
	printf("Porta: %ld\n", porta);

	for(int i=0; i<500000000; i++){
		for(int j; j<500000000; j++){}
	}

	close(socket_servidor);         
	socket_servidor = socket(AF_INET,SOCK_STREAM,0);

	if(socket_servidor == -1){
		printf("Falha ao criar socket\n");
		return -1;
	}


	//Conecta na porta fornecida pela base
	//Funções do <netinet.h>
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234+porta);   //Porta de conexao
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//fim

	memset(&(addr.sin_zero),0,sizeof(addr.sin_zero));

	printf("Tentando se conectar a base...\n");

	if(connect(socket_servidor,(struct sockaddr*)&addr,sizeof(addr)) == -1)  {
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
	}while(enviados != -1); // Enquanto nao ha retorno de erro


	close(socket_servidor);         
	return 0;
}