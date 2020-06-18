#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include "moving.h"

#define MAXLINE 1024 

/*
* Here I send a datagram to a receiver whose name I get from the
* command line arguments. The form of the command line is:
* dgramsend hostname portnumber
*/

void main(int argc, char* argv[]) { 
	int sock; 
	char buf[MAXLINE]; 
  char answer[2];
  int n, len; 
  char m;
  int a = 0;
	struct sockaddr_in	 server; 
  struct addrinfo *hp,hints;

	//struct hostent *hp, *gethostbyname();
  /* Create socket on which to send. */
  sock = socket(AF_INET,SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }

  /*
  * Construct name, with no wildcards, of the socket to ‘‘send’’
  * to. gethostbyname returns a structure including the network
  * address of the specified host. The port number is taken from
  * the command line.
  */

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
 
  int res = getaddrinfo(argv[1],argv[2],&hints,&hp);
  if (res != 0) {
    fprintf(stderr, "%s: getting address error\n", argv[1]);
    exit(2);
  }
  
  cbreak();   //Colocar o terminal no modo cbreak ou rare
  for (int i=0; i<2; i++){    // esse ciclo serve para o cursor inicializar no S (Posição inicial)
    if (sendto(sock,"f", sizeof "f" ,0, hp->ai_addr,hp->ai_addrlen) == -1)
      perror("sending datagram message");
    }
	
  while (a == 0){    
    answer[0] = getchar();      //recebe caracter inserido pelo utilizador
	  sendto(sock, (const char *)answer, strlen(answer),            //envia o caracter para o server
		  0, hp->ai_addr, hp->ai_addrlen); 

	  n = recvfrom(sock, (char *)buf, MAXLINE, 
			MSG_WAITALL, (struct sockaddr *) &server, &len);      //recebe a resposta do servidor

	  buf[n] = '\0'; 
	  printf("%.3s %c\n", buf,answer[0]);   //mostra no ecrã a resposta do utilizador

    m = answer[0];
    switch(m){
      case 'q':     // termina o programa se for premida a tecla "q"
      case 'Q':
        nocbreak();  // termina o modo cbreak
	      close(sock);
	      exit(0);
      }
    }
    
	close(sock); 
	exit(0);

} 