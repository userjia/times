#include "mian.h"
struct device dev;

void getSocket(char *socketType){
    int sockfd;
    char buf[128];
    ssize_t len;
    struct sockaddr_in addr=dev.addr;
    struct sockaddr_in their_addr;
    socklen_t socklen=sizeof(struct sockaddr);

    bzero(&addr, sizeof(addr));//TODO what is bzero,htons?
    addr.sin_family=PF_INET;
    addr.sin_port=htons(atoi(port));
    addr.sin_addr.s_addr=inet_addr(input_addr);

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("create socket");
        return -1;
    }

    if (strcmp(socketType,"server")==0){
        if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1){
            perror("bind");
            return -1;
        } else{
            printf("bind success, listening\n");
            len=recvfrom(dev.fd,buf,sizeof(buf)-1,0,(struct sockaddr *) &dev.their_addr,&addr_len);
            if (len!=-1) {
                printf("get connection from %s",their_addr);
            }
        }

    }else if(strcmp(socketType,"client")==0){

        if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1){
            perror("bind");
            return -1;
        }////

        dev.addr=addr;
        memset(buf,'\0',sizeof(buf));
        buf=addr;
        len=sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *) &dev.addr, sizeof(dev.addr));
    }

    dev.fd=sockfd;
}
