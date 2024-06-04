#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
//#include <linux/tcp.h>
//#include <linux/ip.h>

#define server_ip "192.168.191.131"
#define server_port 8888 

#define MES_BUFSIZE 20



struct Client_Info
{
    int client_fd;
    struct sockaddr_in client_addr;    
};


void *fun_thrReceiveHandler(void *socketInfo);
void received_card(char *card);
void received_request_card(struct Client_Info c_Info);

pthread_t arrThrReceiveClient[10];
int thrReceiveClientCount = 0;

//消息队列
struct Mes
{
    long type;
    char text[MES_BUFSIZE];
};
static int msg_id = -1; //消息队列id

int main(int argc,char *argv[])
{
    //消息队列初始化
    msg_id = msgget((key_t)1234, IPC_CREAT | 0666);
    if (msg_id == -1)
    {
        fprintf(stderr, "msgget failed error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    /*服务器初始化*/
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    int client_fd = -1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char send_buf[100] = "hello,I am server";
    int ret = -1;
    if (server_fd < 0)
    {
        perror("socket");
        exit(-1);
    }
    //绑定ip和端口
    memset(&server_addr,0x0,sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);

    ret = bind(server_fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
    if (ret < 0)
    {
        perror("bind");
        close(server_fd);
        exit(-1);
    }
    
    //开始监听
    ret = listen(server_fd,5);
    if (ret < 0)
    {
        perror("listen");
        close(server_fd);
        exit(-1);
    }

    while (1)
    {
        //阻塞接收请求
        memset(&client_addr,0x0,sizeof(struct sockaddr_in));
        socklen_t len = sizeof(struct sockaddr);
        client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&len);
        if (client_fd < 0)
        {
            perror("accept");
            close(server_fd);
            exit(-1);
        }
        //创建子线程
        struct Client_Info info;
        info.client_addr = client_addr;
        info.client_fd = client_fd;

        pthread_t thrReceive = 0;
        pthread_create(&thrReceive,NULL,fun_thrReceiveHandler,&info);
        arrThrReceiveClient[thrReceiveClientCount++] = thrReceive;
    }


    /*
    //打印客户端信息
    char c_addr[20];
    inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,c_addr,sizeof(c_addr));
    printf("服务器:客户端:IP <%s> port<%d> 连接到服务器！\n",c_addr,client_addr.sin_port);




    //发送信息给客户端
    ret = send(client_fd,&send_buf,strlen(send_buf),0);
    if (ret < 0)
    {
        perror("recv");
        close(server_fd);
        close(client_fd);
        exit(-1);
    }
    
    close(server_fd);
    close(client_fd);
*/    
    return 0;
}


void *fun_thrReceiveHandler(void *socketInfo)
{
    struct Client_Info clinentInfo = *(struct Client_Info*)socketInfo;
    int len = 0;
    char buffer[100];
    char success_mes[100];

    //打印客户端信息
     char c_addr[20];
    inet_ntop(AF_INET,&clinentInfo.client_addr.sin_addr.s_addr,c_addr,sizeof(c_addr));
    printf("服务器:客户端:IP <%s> port<%d> 连接到服务器！\n",c_addr,clinentInfo.client_addr.sin_port);

    //发送连接成功
    sprintf(success_mes,"服务器连接成功!");
    send(clinentInfo.client_fd,success_mes,strlen(success_mes),0);

    while(1)
    {
        //接收客户端消息
        memset(buffer,0x0,sizeof(buffer));
        len = recv(clinentInfo.client_fd,buffer,sizeof(buffer),0);
        if(len < 0)
        {
            printf("服务器:向客户端:IP <%s> port<%d> 读取失败\n",c_addr,clinentInfo.client_addr.sin_port);
            perror("recv");
        }

        //判断客户端有没有离线
        struct tcp_info info;  
        int tcp_info_len = sizeof(info);
        getsockopt(clinentInfo.client_fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&tcp_info_len); 
        if((info.tcpi_state != TCP_ESTABLISHED))
        {
            printf("服务器:客户端:IP <%s> port<%d> 断开连接\n",c_addr,clinentInfo.client_addr.sin_port);
            close(clinentInfo.client_fd);
            pthread_exit(NULL);
        }

        printf("服务器:向客户端:IP <%s> port<%d> 读取成功\n",c_addr,clinentInfo.client_addr.sin_port);
        printf("数据：%s\n",buffer);

        //接收到卡号
        if (strncmp(buffer,"卡号",2)== 0)
        {
            received_card(buffer);
        }
        /*接收到卡号请求*/
        if (strncmp(buffer,"请求卡号",8) == 0)
        {
            received_request_card(clinentInfo);
        }
        


    } 
}
/*接收stm32发来的卡号，写入消息队列*/
void received_card(char *card)
{
    struct Mes data;
    data.type = 1;
    strcpy(data.text,card);

    //向队列里发送消息
    if (msgsnd(msg_id,(void*)&data,MES_BUFSIZE,0) == -1)
    {
        fprintf(stderr, "msgsnd failed\n");
        exit(EXIT_FAILURE);
    }
    //打印写入成功日志
    printf("卡号写入消息队列成功：{type: %ld ; txet: %s\n}",data.type,data.text);
}
/*客户端请求卡号*/
void received_request_card(struct Client_Info c_Info)
{
    struct Mes data;
    if( msgrcv(msg_id,(void*)&data,MES_BUFSIZE,1,0) == -1 )
    {
        fprintf(stderr, "msgrcv failed width erro: %d", errno);
    }

    if(send(c_Info.client_fd,(void*)&data.text,strlen(data.text),0) < 0)
    {
        fprintf(stderr, "send failed width erro: %d\n", errno);
    }
     
    char c_addr[20];
    inet_ntop(AF_INET,&c_Info.client_addr.sin_addr.s_addr,c_addr,sizeof(c_addr));
    printf("服务器:向客户端:IP <%s> port<%d> 发送:%s 成功\n",c_addr,c_Info.client_addr.sin_port,data.text);
}
