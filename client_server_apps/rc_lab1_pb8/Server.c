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

    uint16_t size2;
    recv(c, &size2, sizeof(size2), MSG_WAITALL);
    size2 = ntohs(size2);
    printf("[SERVER] I got the size of the 2nd array: %d\n", size2);

    uint16_t elems2[size2];
    for(int i = 0; i < size2; i++){
        recv(c, &nr, sizeof(nr), MSG_WAITALL);
        nr = ntohs(nr);
        elems2[i] = nr;
        }

    printf("[SERVER] I got the next array:\n");
    for(int i = 0; i < size2; i++){
        printf("%d ", elems2[i]);
    }
    int k = 0;
    uint16_t final[size + size2];
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size2; j++){
            if(elems[i] == elems2[j]){
                final[k] = elems[i];
                k++;
            }
        }
    }
    k = htons(k);
    send(c, &k, sizeof(k), 0);
    k = ntohs(k);
    for(int i = 0; i < k; i++){
        final[i] = htons(final[i]);
        send(c, &final[i], sizeof(final[i]), 0);
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