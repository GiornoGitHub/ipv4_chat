#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_MSG_LEN 1024
#define MAX_NICK_LEN 100

int port;
char nick[MAX_NICK_LEN];
void* sender(void* data);
void* reciever(void* data);

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Ошибка! Введите данные в нужном формате!");
    }
    strncpy(nick, argv[2], MAX_NICK_LEN-1);
    nick[MAX_NICK_LEN-1]='\0';
    printf("Добро пожаловать : %s", argv[2]);
    pthread_t thread_sender, thread_reciever;
    port = atoi(argv[1]);
    pthread_create(&thread_reciever, NULL, reciever, NULL);
    pthread_create(&thread_sender, NULL, sender, NULL);
    pthread_join(thread_sender, NULL);
    pthread_join(thread_reciever, NULL);
    
}

void* sender(void* data){
    char full_msg[MAX_MSG_LEN+MAX_NICK_LEN+2];
    char buffer[MAX_MSG_LEN];
    int socket_s;
    struct sockaddr_in br_addr;
    socket_s = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(socket_s, SOL_SOCKET, SO_BROADCAST, (int*)1, sizeof(int));
    memset(&br_addr, 0, sizeof(br_addr));
    br_addr.sin_family = AF_INET;
    br_addr.sin_addr.s_addr = INADDR_ANY;
    br_addr.sin_port = htons(port);
    bind(socket_s, (struct sockaddr*)&br_addr, sizeof(br_addr));
    while(1){
        fgets(buffer, MAX_MSG_LEN, stdin);
        snprintf(full_msg, sizeof(full_msg), "%s:%s", nick, buffer);
        if(sendto(socket_s, full_msg, strlen(full_msg), 0, (struct sockaddr*)&br_addr, sizeof(br_addr)) == -1){
            perror("send failed");
        };
    }
    close(socket_s);
    return NULL;
}

void* reciever(void* data){
    char buffer[MAX_MSG_LEN + MAX_NICK_LEN+1];
    int socket_r;
    struct sockaddr_in my_addr, other_addr;
    socklen_t addr_len = sizeof(other_addr);
    socket_r = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(socket_r, SOL_SOCKET, SO_BROADCAST, (int*)1, sizeof(int));
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(port);
    bind(socket_r, (struct sockaddr*)&my_addr, sizeof(my_addr));
    while(1){
        int msg_b = recvfrom(socket_r, buffer, sizeof(buffer), 0, (struct sockaddr*)&other_addr, &addr_len);
        if(msg_b > 0){
            buffer[msg_b] = '\0';
            char* msg = buffer;
            printf("[%s]%s", inet_ntoa(other_addr.sin_addr), msg);
        }
    }
    close(socket_r);
    return NULL;
}