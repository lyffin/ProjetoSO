#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#include "moving.h"
#include "reading.h"
#include "jogo.h"

#define MAXLINE 1024 

/*
* The include file <netinet/in.h> defines sockaddr_in as:
* struct sockaddr_in {
* short sin_family;
* u_short sin_port;
* struct in_addr sin_addr;
* char sin_zero[8];
* };
* This program creates a datagram socket, binds a name to it, then
* reads from the socket.
*/

pthread_t cons[1];  //numero de threads
pthread_t prod[1];
pthread_mutex_t buffer_mutex;
char buffer[MAXLINE];
int prod_pos=0, cons_pos=0;
sem_t free_positions, filled_positions;

void *consumidor(void *arg);
void *produtor(void *arg);
void *consumidorB(void *arg);
void *produtorB(void *arg);
int **maze, *pos, *dir;

void main() { 
	int sock; 
	char buf[MAXLINE]; 
	char *anwser = "Ok! "; 
	struct sockaddr_in server, client; 
  int len, n;
  int a = 0;

	pthread_mutex_init(&buffer_mutex, NULL);  //inicializa um mutex
  sem_init(&free_positions, 0, MAXLINE);   
  sem_init(&filled_positions, 0, 0);

	/* Create socket from which to read. */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }
	
	/* Create name with wildcards. */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;   //name.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = 0;    //name.sin_port = htons(8010);
	
	// Bind the socket with the server address 
	if (bind(sock,(struct sockaddr *)&server, sizeof server) == -1) {
    perror("binding datagram socket");
    exit(1);
  }

  /* Find assigned port value and print it out. */
	len = sizeof(client); //len is value/resuslt 

  if (getsockname(sock,(struct sockaddr *) &server, &len) == -1) {
    perror("getting socket name");
    exit(1);
  }

  printf("Socket port #%d\n", ntohs( server.sin_port));   //mostra o socket port
  n = recvfrom(sock, (char *)buf, MAXLINE, 
		MSG_WAITALL, ( struct sockaddr *) &client, &len); 
    
  char resposta[2];
  printf("Indique qual é a alinea que quer testar.\nA - Modo normal.\nB - Modo 3 teclas.\n"); //menu
  resposta[0] = getchar(); //recebe a resposta do client

  switch (resposta[0]){
    case 'a':             //modo normal
    case 'A':
      maze = jogo(0,0);   // recebe/inicia o mapa
      pos = posi(maze);   // recebe a posição do cursor 
      
      while (a == 0){
	      n = recvfrom(sock, (char *)buf, MAXLINE, 
				  MSG_WAITALL, ( struct sockaddr *) &client, &len); 

	      buf[n] = '\0'; 
        
        pthread_create(&(cons[0]), NULL, produtor, buf);          //cria a thread produtor
        pthread_create(&(prod[0]), NULL, consumidor, buf);        //cria a thread consumidor
        pthread_join(cons[0], NULL);
        pthread_join(prod[0], NULL);

	      sendto(sock, (const char *)anwser, strlen(anwser), 
		    MSG_CONFIRM, (const struct sockaddr *) &client, len);       
      }
  
    case 'b':             //modo 3 teclas
    case 'B':
      maze = jogo(0,0);   // recebe/inicia o mapa
      pos = posi(maze);   // recebe a posição do cursor 
      dir = direcao();
      printf("\033[%d;%dH^",32+2-pos[1],pos[0]);
      printf("\033[%d;%dH",32+2-pos[1],pos[0]);
  
      while (a == 0){
	      n = recvfrom(sock, (char *)buf, MAXLINE, 
				  MSG_WAITALL, ( struct sockaddr *) &client, &len); 
	      buf[n] = '\0'; 

        pthread_create(&(cons[0]), NULL, produtorB, buf);
        pthread_create(&(prod[0]), NULL, consumidorB, buf);
        pthread_join(cons[0], NULL);
        pthread_join(prod[0], NULL);
       
	      sendto(sock, (const char *)anwser, strlen(anwser), 
	  	    MSG_CONFIRM, (const struct sockaddr *) &client, len); 
      } 
  
	close(sock);
  exit(0);
  } 
}

//threads produtor/consumidor modo normal
void *produtor(void *arg) {
  int n;
  int *iptr = (int *)arg;

  sem_wait(&free_positions);  // dá lock no semaphone
  pthread_mutex_lock(&buffer_mutex);
  buffer[prod_pos] = *iptr;
  prod_pos = (prod_pos+1) % MAXLINE;
  pthread_mutex_unlock(&buffer_mutex);  // dá unlock
  sem_post(&filled_positions);
}

void *consumidor(void *arg) {
  int n;

  sem_wait(&filled_positions);
  pthread_mutex_lock(&buffer_mutex);
  n = buffer[cons_pos];
  cons_pos = (cons_pos+1) % MAXLINE;
  pthread_mutex_unlock(&buffer_mutex);
  sem_post(&free_positions);
  move(maze, (char *)&n, pos);  //move o cursor
}

//threads produtor consumidor modo 3 teclas
void *produtorB(void *arg) {
  int n;
  int *iptr = (int *)arg;
        
  sem_wait(&free_positions);
  pthread_mutex_lock(&buffer_mutex);
  buffer[prod_pos] = *iptr;
  prod_pos = (prod_pos+1) % MAXLINE;
  pthread_mutex_unlock(&buffer_mutex);
  sem_post(&filled_positions);
}

void *consumidorB(void *arg) {
  int n;
        
  sem_wait(&filled_positions);      
  pthread_mutex_lock(&buffer_mutex);
  n = buffer[cons_pos];
  cons_pos = (cons_pos+1) % MAXLINE;
  pthread_mutex_unlock(&buffer_mutex);
  sem_post(&free_positions);
  moveb(maze, (char *)&n, pos, dir); //move o cursor
}