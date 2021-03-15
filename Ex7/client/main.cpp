

// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netdb.h>

#include <iostream>

using namespace std;

#define PORT 9000 
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 

    struct sockaddr_in servaddr; 
    struct hostent *server;

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT);
	server = gethostbyname("10.0.0.1");
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	printf("Server name: %s\n", server->h_name);

	//Cope adress and size from hostens struct to sockaddr struct.
    bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);


    int n; 
    socklen_t len;
    cout << "Ask for uptime (U/u) or loadavg (L/l)" << endl;
    cin >> buffer;

    sendto(sockfd, (const char *)buffer, strlen(buffer), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("command message sent: %s\n", buffer); 
          
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
        MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
} 
