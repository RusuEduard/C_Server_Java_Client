#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void handle_client(int c){
    uint16_t size;
    recv(c, &size, sizeof(size),  MSG_WAITALL);
    size = ntohs(size);
    printf("[SERVER] String has lenght: %d\n", size);
    char string[size];
    recv(c, &string, size, MSG_WAITALL);
    printf("[SERVER] String received: %s\n", string);
    char character;
    recv(c, &character, sizeof(character), MSG_WAITALL);
    printf("[SERVER] Character received is: %c\n", character);
    
    uint16_t count = 0;
    uint16_t poz[size];

    for(int i = 0; i < size; i++){
        if(string[i] == character){
            poz[count] = i;
            count++;
        }
    }
    printf("[SERVER] Number of appearances: %d\n", count);
    count = htons(count);
    send(c, &count, sizeof(count), 0);
    count = ntohs(count);

    for(int i = 0; i < count; i++){
        poz[i] = htons(poz[i]);
        send(c, &poz[i], sizeof(poz[i]), 0);
    }

    close(c);

}

int main(int argc, char* argv[]){
    int s, c, l;
    struct sockaddr_in server, client;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if( s < 0){
        printf("[SERVER] Eroare la crearea socketului!\n");
        return(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(5050);
    server.sin_addr.s_addr = INADDR_ANY;

    if( bind(s, (struct sockaddr*) &server, sizeof(server)) < 0){
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
    close(s);
}