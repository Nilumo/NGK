//============================================================================
// Name        : file_server.cpp
// Author      : Nikolaj Lonka & Nikolaj Lund
// Version     : 1.0
// Description : file_server in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cmath>

#include "iknlib.h"

using namespace std;

#define buf_size 1000
#define send_size 2

void sendFile(string fileName, long fileSize, int outToClient);

/**
 * main starter serveren og venter på en forbindelse fra en klient
 * Læser filnavn som kommer fra klienten.
 * Undersøger om filens findes på serveren.
 * Sender filstørrelsen tilbage til klienten (0 = Filens findes ikke)
 * Hvis filen findes sendes den nu til klienten
 *
 * HUSK at lukke forbindelsen til klienten og filen nÃ¥r denne er sendt til klienten
 *
 * @throws IOException
 *
 */
int main(int argc, char *argv[])
{
    char path_buf[buf_size] {}, buffer[buf_size] {};
    int portno = 9000, socketfd, c_socketfd, err;
    struct sockaddr_in serv_addr, cli_addr;


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) 
        error("ERROR opening socket");
    
    //Bind fd and portno
    if (bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    printf("Starts to listen\n");
    //Listens for client initiation
    err = listen(socketfd, 1);
        if(err < 0)
            printf("Error, could not listen\n");
    
    //saves size in socklen_t struct
    socklen_t c_size = sizeof(cli_addr);

    while(1) { //Revurdere om den skal loop mere/mindre.
        printf("Waiting for client accept\n");
        c_socketfd = accept(socketfd, (struct sockaddr *) &cli_addr, &c_size);
        printf("Accepted\n");

        read(c_socketfd, path_buf, buf_size);
        printf("Filename: %s\n", path_buf);
        
        int long file_size;
        file_size = check_File_Exists(path_buf);
        printf("Filesize: %ld\n", file_size);
        if (file_size == 0) {
            error("File does not exist\n");
            write(c_socketfd,"File does not exist", 19+1);
        }
        else {
            //***Send size of file to client ***
            err = sprintf(buffer, "%ld", file_size);
            if(err < 0 )
                printf("sccan errno: %d\n", err);


            write(c_socketfd, buffer, strlen(buffer) + 1);

            //*** Send file to client in packages of 1000 bytes ***
            //amount of packages
            int cycles = round(file_size/buf_size +0.5);
            int ret;
            printf("Cycles: %d\n", cycles);

            //Input file = read from file
            ifstream myFile;
            myFile.open(path_buf, ios::in | ios::binary);

            //Runs amount of packages
            for (int i = 0; i < cycles; i++) 
            {
                //Read from pointer and onwards 1000 bytes
                myFile.read(buffer, buf_size); //change
                
                //Write file package to client
                //cout << buffer;
                //ret = write(c_socketfd, buffer, strlen(buffer));
                ret = send(c_socketfd, buffer, strlen(buffer), 0);
                if (ret < 0) 
                    cout << "ERROR writing to socket\n";
                //Set pointer in file
                //myFile.seekg(strlen(buffer), myFile.cur);
                //cout << strlen(buffer) << ":" << buffer << endl;
                cout << "Buffer: \n"  << buffer << "\nReturnvalue: " << ret << endl;
                //cout << "Package" << (i+1) << endl;
            }
            myFile.close();
            close(c_socketfd);  
        }
    }
    
    close(socketfd);

    return 0;
}

/**
 * Sender filen som har navnet fileName til klienten
 *
 * @param fileName Filnavn som skal sendes til klienten
 * @param fileSize Størrelsen på filen, 0 hvis den ikke findes
 * @param outToClient Stream som der skrives til socket
     */
void sendFile(string fileName, long fileSize, int outToClient)
{
    // TO DO Your own code
}

