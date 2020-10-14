#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#include <unistd.h>
#include <string.h>

void handle_client(int c){
    uint16_t len, count;
    char b[100];
    recv(c, &len, sizeof(len), MSG_WAITALL);
    len = ntohs(len);
    printf("[SERVER] Dimensiunea sirului este: %d\n", len);
    recv(c, b, len, MSG_WAITALL);
    printf("[SERVER] Sirul este: %s\n", b);
    count = 0;
    for(int i = 0; i < len; i++){
        if(b[i] == ' '){
            count++;
        }
    }
    printf("[SERVER] Count is %d\n", count);
    count = htons(count);
    send(c, &count, sizeof(count), 0);
    close(c);
}

int main(){
    int s, c, l;
    struct sockaddr_in server, client;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("Eroare la crearea serverului!\n");
        return(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(5050);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(s, (struct sockaddr*) &server, sizeof(server))<0){
        printf("Eroare la bind!\n");
        return(1);
    }

    listen(s, 5);
    l = sizeof(client);
    memset(&client, 0, sizeof(client));

    int running = 1;
    while(running == 1){
        c = accept(s, (struct sockaddr*) &client, &l);
        printf ("S-a conectat un client!\n");
        if(fork() == 0){
            handle_client(c);
            exit(0);
        }
        signal(SIGCHLD, SIG_IGN);
    }

    close(s);
    return 0;
}