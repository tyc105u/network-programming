#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>

typedef struct sockaddr *sockaddrp;
int sockfd;
char secret;
int mode = 0; //0 for common  ,1 for send
FILE *fp;
char filename[50] = "download_";
char fname[50];
void *recv_other(void *arg)
{
    char buf[255] = {};
    while (1)
    {
        char yn = 0;
        char buf[255] = {};
        char buf2[1024] = {};
        char buf3[255] = {};
        char buf4[255] = {};
        int ret = recv(sockfd, buf, sizeof(buf), 0);

        if (strcmp("send", buf) == 0)
        {
            int nCount;
            printf("Recive? y/NO \n");
   //         scanf("%s", buf3);
   //         if(strcmp(buf3,"NO")==0)
   //             continue;
            recv(sockfd, buf4, 255, 0);
            if (strcmp(buf4, "eof") == 0)
            {}
            else
            {
                fp = fopen(buf4, "wb");
                while ((nCount = recv(sockfd, buf2, 1024, 0)) > 0)
                {
                    fwrite(buf2, nCount, 1, fp);
                    if (strcmp(buf2, "eof") == 0)
                        break;
                }
                printf("END\n");
                fclose(fp);
                continue;
            }
            sprintf(buf4, "");
            recv(sockfd, buf4, 255, 0);
            printf("%s", buf4);
        }
        else
        {
            if (0 > ret)
            {
                perror("recv");
                return 0;
            }
            printf("%s\n", buf);
        }
    }
}

int main(int argc, char **argv)
{
    if (3 != argc)
    {
        perror("參數錯誤");
        return -1;
    }
    //建立socket對象
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sockfd)
    {
        perror("socket");
        return -1;
    }
    //準備連接地址
    struct sockaddr_in addr = {AF_INET};
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    socklen_t addr_len = sizeof(addr);

    //連接
    int ret = connect(sockfd, (sockaddrp)&addr, addr_len);
    if (0 > ret)
    {
        perror("connect");
        return -1;
    }

    //發送名字
    char buf[255] = {};
    char name[255] = {};
    printf("請輸入您的昵稱：");
    scanf("%s", name);
    strcpy(fname, name);
    ret = send(sockfd, name, strlen(name), 0);
    if (0 > ret)
    {
        perror("connect");
        return -1;
    }

    //創建接收子線程
    pthread_t tid;
    ret = pthread_create(&tid, NULL, recv_other, NULL);

    if (0 > ret)
    {
        perror("pthread_create");
        return -1;
    }
    //循環發送
    while (1)
    {
        // printf("%s:",name);
        scanf("%s", buf);
        int ret = send(sockfd, buf, strlen(buf), 0);
        if (0 > ret)
        {
            perror("send");
            return -1;
        }
        //輸入quit退出
        if (0 == strcmp("quit", buf))
        {
            printf("%s,您已經退出了聊天室\n", name);
            return 0;
        }
        else if (strstr(buf,"send") != NULL)
        {
            FILE *copy = fopen("1.jpeg", "rb");
            int nCount;
            char buf_snd4[1024]={0},buf_rcv2[256]={0};
            printf("wait...\n");
            while ((nCount = fread(buf_snd4, 1, 1024, copy)) > 0)
            {
                send(sockfd, buf_snd4, nCount, 0);
            }
            printf("success\n");
            recv(sockfd, buf_rcv2, 255, 0);
            send(sockfd, "eof", sizeof("eof"), 0);
            fclose(fp);
        }
    }
}