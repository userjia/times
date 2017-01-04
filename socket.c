#include "main.h"
struct device dev;

int getSocket(){
    int sockfd;
    struct sockaddr_in addr=dev.addr;

    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
        perror("create socket");
        return -1;
    }
    if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1){
        perror("bind");
        return -1;
    }
    dev.fd=sockfd;
    return 0;
}

int confirmSock(int character){
    int sockfd;
    char buf[128];
    char c[2];
    ssize_t len;
    struct sockaddr_in their_addr;
    socklen_t socklen=sizeof(struct sockaddr);
    char *socketType;
    sockfd=dev.fd;
    if (character==host){
        socketType="server";
    }else{
        socketType="client";
    }

    if (strcmp(socketType,"server")==0){
        printf("bind success, listening %d\n",dev.serverTime);
            len=recvfrom(sockfd,buf,sizeof(buf)-1,0,(struct sockaddr *) &their_addr,&socklen);
            if (len!=-1) {
                printf("get connection from %s\n",inet_ntoa(their_addr.sin_addr));
                dev.their_addr=their_addr;

            } else{
                perror("receive");
                return -1;
            }
            memset(buf,'\0', sizeof(buf));
            strcpy(buf,"ok");
            len=sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *) &dev.their_addr,sizeof(dev.their_addr));
            if (len!=-1){
                return 0;
            }

    }else if(strcmp(socketType,"client")==0){

            strcpy(buf,"sync time");
            len=sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *) &dev.their_addr,sizeof(dev.their_addr));
            if(len!=-1){
                printf("send success, wait to receive.\n");
                if(dev.waitRecv==1){
                    sleep(1);
                }
                memset(buf,'\0', sizeof(buf));
                len=recvfrom(sockfd,buf,sizeof(buf)-1,MSG_DONTWAIT,(struct sockaddr *) &their_addr,&socklen);
                if (len!=-1) {
                    printf("get connection from %s\n",inet_ntoa(their_addr.sin_addr));
                    if(dev.their_addr.sin_addr.s_addr!=their_addr.sin_addr.s_addr){
                        printf("undesired server, waiting for the right?(y/n):");
                        exit(-4);
                    } else if(strcasecmp(buf,"ok")==0){
                        printf("start sync.\n");
                        dev.waitRecv=0;
                        return 0;
                    } else{
                        printf("confirm wrong.\n");
                        return -1;
                    }
                }else{
                    perror("socket recv");
                    return -1;
                }
            } else{
                printf("send to server failed\n");
                return -1;
            }

    }
}
