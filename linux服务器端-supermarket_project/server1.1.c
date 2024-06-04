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
#include <errno.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <linux/tcp.h>
//#include <linux/ip.h>

#define server_ip "192.168.67.15"
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
void received_request_memlist(struct Client_Info c_Info);
void received_request_goodslist(struct Client_Info c_Info);
void received_request_save_memlist(char *memlist);
void received_request_save_goodslist(char *goodslist);

pthread_t arrThrReceiveClient[10];
int thrReceiveClientCount = 0;

//card 缓存区
struct Card_Msg
{
    int ready_flag;
    char text[MES_BUFSIZE];
};
static pthread_mutex_t mutex; //定义互斥锁
static pthread_cond_t cond; //定义条件变量
static struct Card_Msg card_data;


int main(int argc,char *argv[])
{
    //条件变量初始化
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    card_data.ready_flag = 0;

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
    char buffer[4096];
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
        if (strncmp(buffer,"请求卡号",12) == 0)
        {
            received_request_card(clinentInfo);
        }
        /*接收到请求会员列表*/
        if(strncmp(buffer,"请求会员列表",18) == 0)
        {
            received_request_memlist(clinentInfo);
        }
        /*接收到请求商品列表*/
        if(strncmp(buffer,"请求商品列表",18) == 0)
        {
            received_request_goodslist(clinentInfo);
        }
        /*接收到请求保存会员列表*/
        if(strncmp(buffer,"请求保存会员列表",24) == 0)
        {
            received_request_save_memlist(buffer+25);
        }
        /*接收到请求保存商品列表*/
        if(strncmp(buffer,"请求保存商品列表",24) == 0)
        {
            received_request_save_goodslist(buffer+25);
        }

    } 
}
/*接收stm32发来的卡号，写入消息队列*/
void received_card(char *card)
{

    //向卡号缓存区写入消息
    pthread_mutex_lock(&mutex);//上锁
    strcpy(card_data.text,card);
    card_data.ready_flag = 1;  //设置条件标志
    pthread_mutex_unlock(&mutex);//解锁
    pthread_cond_signal(&cond);//向条件变量发送信号
    //打印写入成功日志
    printf("卡号写入缓存区成功：%s\n",card_data.text);
}
/*客户端请求卡号*/
void received_request_card(struct Client_Info c_Info)
{
    char data[MES_BUFSIZE];

    pthread_mutex_lock(&mutex);//上锁
    while (card_data.ready_flag == 0)
    {
        pthread_cond_wait(&cond, &mutex);//等待条件满足
    }
    strcpy(data,card_data.text);
    card_data.ready_flag = 0;
    pthread_mutex_unlock(&mutex);//解锁

    if(send(c_Info.client_fd,(void*)&data,strlen(data),0) < 0)
    {
        fprintf(stderr, "send failed width erro: %d\n", errno);
    }
     
    char c_addr[20];
    inet_ntop(AF_INET,&c_Info.client_addr.sin_addr.s_addr,c_addr,sizeof(c_addr));
    printf("服务器:向客户端:IP <%s> port<%d> 发送:%s 成功\n",c_addr,c_Info.client_addr.sin_port,data);
}
/*客户端请求会员列表*/
void received_request_memlist(struct Client_Info c_Info)
{
    int fd = -1;
    int ret = -1;
    char read_buffer[4096];
    char send_buffer[4096];
    memset(read_buffer,0,sizeof(read_buffer));
    memset(send_buffer,0,sizeof(send_buffer));
    //打开会员列表
    fd = open("memlist.csv",O_RDONLY);
    if(fd < 0)
    {
        perror("memlist.csv open failed");
        return;
    }
    //读取会员列表内的数据   
    ret = read(fd,read_buffer,sizeof(read_buffer));
    if(ret < 0)
    {
        perror("memlist.csv read failed");
        return;
    }
    close(fd);


    sprintf(send_buffer,"会员列表:%s",read_buffer);

    //转发会员数据
    if(send(c_Info.client_fd,(void*)&send_buffer,strlen(send_buffer),0) < 0)
    {
        fprintf(stderr, "send failed width erro: %d\n", errno);
    }

    char c_addr[20];
    inet_ntop(AF_INET,&c_Info.client_addr.sin_addr.s_addr,c_addr,sizeof(c_addr));
    printf("服务器:向客户端:IP <%s> port<%d> 发送:%s 成功\n",c_addr,c_Info.client_addr.sin_port,send_buffer);
    
}
/*请求商品列表*/
void received_request_goodslist(struct Client_Info c_Info)
{
    int fd = -1;
    int ret = -1;
    char read_buffer[4096];
    char send_buffer[4096];
    memset(read_buffer,0,sizeof(read_buffer));
    memset(send_buffer,0,sizeof(send_buffer));

    //打开会员列表
    fd = open("goodslist.csv",O_RDONLY);
    if(fd < 0)
    {
        perror("goodslist.csv open failed");
        return;
    }
    //读取会员列表内的数据   
    ret = read(fd,read_buffer,sizeof(read_buffer));
    if(ret < 0)
    {
        perror("goodslist.csv read failed");
        return;
    }
    close(fd);


    sprintf(send_buffer,"商品列表:%s",read_buffer);

    //转发会员数据
    if(send(c_Info.client_fd,(void*)&send_buffer,strlen(send_buffer),0) < 0)
    {
        fprintf(stderr, "send failed width erro: %d\n", errno);
    }

    char c_addr[20];
    inet_ntop(AF_INET,&c_Info.client_addr.sin_addr.s_addr,c_addr,sizeof(c_addr));
    printf("服务器:向客户端:IP <%s> port<%d> 发送:%s 成功\n",c_addr,c_Info.client_addr.sin_port,send_buffer);
}
/*请求保存会员列表*/
void received_request_save_memlist(char *memlist)
{
    int fd = -1;
    int ret = -1;
    char write_buffer[4096];
    strcpy(write_buffer,memlist);

    //打开会员列表
    fd = open("memlist.csv",O_RDWR|O_TRUNC);
    if(fd < 0)
    {
        perror("memlist.csv open failed");
        return;
    }
    //写入数据
    ret = write(fd,write_buffer,strlen(write_buffer));
    if (ret < 0)
    {
        perror("memlist.csv write failed");
        return;
    }

    printf("会员列表写入成功\n");
    close(fd);
    
}
/*请求保存商品列表*/
void received_request_save_goodslist(char *goodslist)
{
    int fd = -1;
    int ret = -1;
    char write_buffer[4096];
    strcpy(write_buffer,goodslist);

    //打开会员列表
    fd = open("goodslist.csv",O_RDWR|O_TRUNC);
    if(fd < 0)
    {
        perror("goodslist.csv open failed");
        return;
    }
    //写入数据
    ret = write(fd,write_buffer,strlen(write_buffer));
    if (ret < 0)
    {
        perror("goodslist.csv write failed");
        return;
    }

    printf("商品列表写入成功\n");
    close(fd);
    
}