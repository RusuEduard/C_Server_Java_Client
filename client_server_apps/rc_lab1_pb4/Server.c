#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <signal.h>

int handle_client(int c){
    u_int16_t len1, len2;

    recv(c, &len1, sizeof(len1), MSG_WAITALL);
    len1 = ntohs(len1);
    printf("[SERVER] Length of 1st string is: %d\n", len1);
    char s1[len1];
    recv(c, s1, len1, MSG_WAITALL);
    printf("[SERVER] 1st string is: %s\n", s1);

    recv(c, &len2, sizeof(len2), MSG_WAITALL);
    len2 = ntohs(len2);
    printf("[SERVER] Length of 2nd string is: %d\n", len2);
    char s2[len2];
    recv(c, s2, len2, MSG_WAITALL);
    printf("[SERVER] 2nd string is: %s\n", s2);
    int len3 = len1 + len2;
    char s3[len3];
    int k=0;
    int i=0;
    if(len1 <= len2){
        while(k<len1-1){
            s3[i] = s1[k];
            s3[i+1] = s2[k];
            k++;
            i+=2;
        }
        while(k<len2){
            s3[i] = s2[k];
            k++;
            i++;
        }
    }
    else{
        while(k<len2-1){
            s3[i] = s1[k];
            s3[i+1] = s2[k];
            k++;
            i+=2;
        }
        while(k<len1){
            s3[i] = s1[k];
            k++;
            i++;
        }
    }

    printf("[SERVER] New string is: %s\n", s3);
    send(c, s3, len3, 0);
    close(c);
}

int main(int argc, char *argv[]){
    int s, c, l;
    struct sockaddr_in server, client;
    
    printf("argv[0]: %s\n", argv[0]);
    printf("argv[1]: %s\n", argv[1]);
    char* ip = argv[1];
    int port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    if(bind(s, (struct sockaddr*) &server, sizeof(server)) < 0){
        printf("Eroare la bind!\n");
        return(1);    
    }


    listen(s, 5);
    l = sizeof(client);
    memset(&client, 0, sizeof(client));
    while(1){
        c = accept(s, (struct sockaddr*) &client, &l);
        printf("Client connected!\n");
        if(fork() == 0){
            handle_client(c);
            exit(0);
        }
        signal(SIGCHLD, SIG_IGN);
    }
    close(s);
    return(0);
}