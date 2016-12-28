#include "main.h"
struct device dev;

int getSocket(char *socketType){
    int sockfd;
    char buf[128];
    char c[2];
    ssize_t len;
    struct sockaddr_in addr=dev.addr;
    struct sockaddr_in their_addr;
    socklen_t socklen=sizeof(struct sockaddr);

    //bzero(&addr, sizeof(addr));//TODO what is bzero,htons?

    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
        perror("create socket");
        return -1;
    }
    if (bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1){
        perror("bind");
        return -1;
    }

    if (strcmp(socketType,"server")==0){

            printf("bind success, listening\n");
            while (1){
                len=recvfrom(sockfd,buf,sizeof(buf)-1,0,(struct sockaddr *) &their_addr,&socklen);
                if (len!=-1) {
                    printf("get connection from %s\n",inet_ntoa(their_addr.sin_addr));
                    dev.their_addr=their_addr;
                } else{
                    perror("receive");
                    printf("try again?(y):\n");
                    scanf("%s",c);
                    if(strcasecmp(c,"y")==0){
                        continue;
                    } else{
                        exit(-3);
                    }
                }
                memset(buf,'\0', sizeof(buf));
                strcpy(buf,"hello server");
                sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *) &dev.their_addr,sizeof(dev.their_addr));
                break;
            }


    }else if(strcmp(socketType,"client")==0){
        while(1){
            //memset(buf,'\0', sizeof(buf));
            strcpy(buf,"hello server");
            //memcpy(buf,h,sizeof(*h));
            len=sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *) &dev.their_addr,sizeof(dev.their_addr));
            if(len!=-1){
                printf("send success, wait to receive.\n");
                break;
            } else{
                printf("send to server failed\n");
                printf("try again?(y):\n");
                scanf("%s",c);
                if(strcasecmp(c,"y")==0){
                    continue;
                } else{
                    exit(-3);
                }
            }
        }

        while(1){
            memset(buf,'\0', sizeof(buf));
            len=recvfrom(sockfd,buf,sizeof(buf)-1,0,(struct sockaddr *) &their_addr,&socklen);
            if (len!=-1) {
                printf("get connection from %s\n",inet_ntoa(their_addr.sin_addr));
            }
            if(dev.their_addr.sin_addr.s_addr!=their_addr.sin_addr.s_addr){
                printf("undesired server, waiting for the right?(y/n):");
                scanf("%s",c);
                if(strcasecmp(c,"y")==0){
                    continue;
                } else{
                    exit(-4);
                }
            } else{
                break;
            }
        }

    }

    dev.fd=sockfd;
}
