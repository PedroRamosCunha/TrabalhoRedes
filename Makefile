#Intrução do compilador
CC=gcc
#parâmetros de compilação do Servidor
servidor=-pthread -I.
#parâmetros de compilação do Cliente
cliente=-lm -I.
COMPILADOR:
#Compila o servidor
	$(CC) -o servidor.out servidor.c $(servidor)
#Compila o cliente
	$(CC) -o cliente.out cliente.c  $(cliente)


