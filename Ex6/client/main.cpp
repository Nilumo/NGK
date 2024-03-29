//============================================================================
// Name        : file_client.cpp
// Author      : Nikolaj Lonka & Nikolaj Lund
// Version     : 1.0
// Description : file_client in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "iknlib.h"
#include <cmath>
#include <arpa/inet.h>
#define buf_size 1000
#define send_size 2
using namespace std;


int main(int argc, char *argv[])
{
	printf("Program started\n");
	printf("Portnumber: 9000 \n");
	int s_socketfd, portno = 9000, err, ret, cycles;
    struct sockaddr_in serv_addr;
	//Fill struct with zeroes
	bzero((char *) &serv_addr, sizeof(serv_addr));
	//struct in_addr addr;

    struct hostent *server;

	char buffer[buf_size] {};

	serv_addr.sin_family = AF_INET;

	s_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (s_socketfd < 0) 
        	fprintf(stderr, "ERROR opening socket\n");


	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	printf("Server name: %s\n", server->h_name);

	//Copy adress and size from hostens struct to sockaddr struct.
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);


	//"The htons() function converts the unsigned short integer hostshort from host byte order to network byte order."
	serv_addr.sin_port = htons(portno);

	//Connect to server
	if (connect(s_socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		printf("Error connecting\n");
	printf("Connected\n\n");

	//*** Send path to server ***
	writeTextTCP(s_socketfd, argv[2]);

	// err = write(s_socketfd, argv[2], strlen(argv[2]+1));
	//     if (err < 0) 
    //     	printf("ERROR writing to socket");
	err = read(s_socketfd, buffer, buf_size);
	if (err < 0) 
		printf("ERROR reading from socket\n");

	int file_size = stoi(buffer, nullptr);

	if(file_size == 0) {
		printf("Error, file doesn't exist\n");
	}
	else
	{
		printf("Filesize: %d\n", file_size);

		cycles = round(file_size/buf_size + 0.5);
		printf("Cycles: %d\n", cycles);

		ofstream myFile;
		//const char *filename_buf = "Copyfile.png";
		myFile.open(argv[2], ios::out | ios::binary);
		
		for (int i = 0; i < cycles; i++) {	
			//Read file package from server
			ret = read(s_socketfd, buffer, buf_size);
			if (ret < 0)
				cout << "Error with read\n";
			else
			{
				myFile.write(buffer, ret);

				cout << "Package " << (i+1) << ": " << ret << " bytes recieved\n";
			}	
		}

		myFile.close();
	}
		
	close(s_socketfd);

	printf("Program done\n");

	return 0;
}