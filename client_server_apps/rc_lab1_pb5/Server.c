#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_client(int c){
    uint16_t nr;
    recv(c, &nr, sizeof(nr), MSG_WAITALL);
    nr = ntohs(nr);
    uint16_t divs[nr];
    uint16_t count = 0;
    int poz = 0;
    for(int i=2; i <= nr/2; i++){
        if(nr % i == 0){
            count++;
            divs[poz] = i;
            poz++;
        }
    }

    if(count == 0){
        count = htons(count);
        send(c, &count, sizeof(count), 0);
        close(c);
    }
    else{
        count = htons(count);
        send(c, &count, sizeof(count), 0);
        for(int i=0; i < poz; i++){
            printf("%d\n", divs[i]);
            divs[i] = htons(divs[i]);
            send(c, &divs[i], sizeof(divs[i]), 0);
        }
        close(c);
    }
}

int main(int argc, char *argv[]){
    int s, c, l;
    struct sockaddr_in server, client;
    int PORT = atoi(argv[2]);
    char* IP = argv[1];

    s = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(IP);

    if(bind(s, (struct sockaddr*) &server, sizeof(server)) < 0){
        printf("Bind error!\n");
        return(1);
    }

    l = sizeof(client);
    listen(s, 5);
    memset(&client, 0, sizeof(client));
    while(1){
        c = accept(s, (struct sockaddr*) &client, &l);
        printf("[Server] Clinet connected!\n");
        if(fork() == 0){
            handle_client(c);
            exit(0);
        }
        signal(SIGCHLD, SIG_IGN);
    }
    close(s);
    return(0);
}