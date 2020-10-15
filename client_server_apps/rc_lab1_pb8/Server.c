#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void handle_client(int c){
    uint16_t size;
    recv(c, &size, sizeof(size), MSG_WAITALL);
    size = ntohs(size);
    printf("[SERVER] I got the size of the array: %d\n", size);
    uint16_t elems[size];
    uint16_t nr;
    for(int i = 0; i < size; i++){
        recv(c, &nr, sizeof(nr), MSG_WAITALL);
        nr = ntohs(nr);
        elems[i] = nr;
    }
    printf("[SERVER] I got the next array:\n");
    for(int i = 0; i < size; i++){
        printf("%d ", elems[i]);
    }
    close(c);
}

int main(int args, char *argv[]){
    int s, c, l;
    struct sockaddr_in server, client;
    

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("[SERVER] Eroare la creare socket!\n");
        return(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5050);

    if(bind(s, (struct sockaddr*) &server, sizeof(server)) < 0){
        printf("[SERVER] Eroare la bind!\n");
        return(1);
    }

    listen(s, 5);
    l = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1){
        c = accept(s, (struct sockaddr*) &client, &l);
        printf("[SERVER] Client connected!\n");
        if(fork() == 0){
            handle_client(c);
            exit(0);
        }
        signal(SIGCHLD, SIG_IGN);
    }

}