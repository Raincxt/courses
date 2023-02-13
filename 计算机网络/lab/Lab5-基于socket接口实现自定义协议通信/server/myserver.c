#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define BACKLOG 5
#define MAX_NAME_LENGTH 256
#define MAX_TEXT_LENGTH 4096

void* threadWork(void* fd); // 函数执行
void disconnect(int clientfd); // 和客户端断开连接
void* endServer(void* ptr); // 服务器关机

struct connect{
    char name[MAX_NAME_LENGTH]; // 客户端名称
    int clientfd;               // 客户端描述符
    char ip[MAX_NAME_LENGTH];   // 客户端ip地址
    int port;                   // 客户端端口
}client[BACKLOG];
int clientNum = 0; // 当前连接的客户端数量
int Port;          // 开放连接的端口号

int main(int argc, char* argv[]) {
    printf("[INFO] Which port do you want to open?\n");
    scanf("%d", &Port);
    printf("\n");

    // 创建服务端的socket
    int serverfd;
    if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("socket");
        return -1;
    }
    printf("[INFO] Open socket successfully.\n");

    // 把服务端用于通信的地址和端口绑定到socket上
    struct sockaddr_in servaddr;                    // 服务端地址信息的数据结构
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                  // 协议族，在socket编程中只能是AF_INET
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // 任意ip地址。
    servaddr.sin_port = htons(Port);       // 指定通信端口。
    if(bind(serverfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { // 绑定
        perror("bind");
        close(serverfd);
        return -1;
    }
    printf("[INFO] Bind socket successfully.\n");
    printf("[INFO] Server starts.\n");
    printf("[INFO] Server is listening...\n\n");

    // 把socket设置为监听模式。
    if(listen(serverfd, BACKLOG) != 0) { // 设置为最大数量监听
        perror("listen"); 
        close(serverfd); 
        return -1;
    }

    // 开一个线程让程序关闭
    pthread_t PID;
    pthread_create(&PID, NULL, endServer, NULL);

    // 接受客户端的连接
    int clientfd; // 客户端的socket
    int socklen = sizeof(struct sockaddr_in); // struct sockaddr_in的大小
    struct sockaddr_in clientaddr; // 客户端的地址信息
    while(1) {
        clientfd = accept(serverfd, (struct sockaddr*)&clientaddr, (socklen_t*)&socklen);
        if(clientfd == -1)
            perror("accept");
        else { // 连接成功，增加用户
            int iret;
            char clientName[MAX_NAME_LENGTH];
            memset(clientName, 0, sizeof(clientName));
            if (iret = recv(clientfd, clientName, sizeof(clientName), 0) <= 0) // 名字错误
                perror("name");
            client[clientNum].clientfd = clientfd; // 客户端描述符
            strcpy(client[clientNum].name, clientName); // 客户端主机名
            strcpy(client[clientNum].ip, inet_ntoa(clientaddr.sin_addr)); // 客户端ip地址
            client[clientNum].port = clientaddr.sin_port; // 客户端端口号
            clientNum++;
            printf("[INFO] New client has connected.\n");
            printf("Client IP: %s\n" , inet_ntoa(clientaddr.sin_addr));
            printf("Client Port: %d\n", clientaddr.sin_port);
            printf("Client Name: %s\n", clientName);
            printf("Client FD: %d\n\n", clientfd);
            pthread_t pid;
            send(clientfd, "hello!", 6, 0); // 发送hello信息给客户端
            pthread_create(&pid, NULL, threadWork, &clientfd); // 创建新线程
        }
    }

    // 关闭socket，释放资源
    close(serverfd);
    printf("[INFO] Close serverfd[%d] successfully.\n", serverfd);
    return 0;
}

// 每个线程处理对应客户端的输入并返回输出
void* threadWork(void* fd) {
    int iret;
    char buffer[MAX_TEXT_LENGTH];
    memset(buffer, 0, sizeof(buffer));
    int clientfd = *(int*)(fd); // 客户端描述符

    // 和客户端交互
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        if (iret = recv(clientfd, buffer, sizeof(buffer), 0) > 0) { // 成功收到消息
            printf("[INFO] Information received from client(%d):%s\n", clientfd, buffer);
            // 处理数据
            switch(buffer[0]) {
                case 's': { // 发送数据, 例如"s 3 Hello world"
                    printf("[INFO] Sending message to other client...\n");
                    int iret;
                    int count = 0;
                    int dstfd = 0; // 目标客户端编号
                    char temp[MAX_TEXT_LENGTH]; // 存储传输内容
                    memset(temp, 0, sizeof(temp));
                    strcpy(temp, ":s ");
                    const char* delim = " "; // 空格作为分隔符切分字符串
                    char* separate_str = NULL; // 存储切分的字符串
                    separate_str = strtok(buffer, delim); // 按照分隔符切分
                    while(separate_str != NULL) {
                        if (count == 0); // s
                        else if (count == 1) // 12, 获得文件描述符
                            dstfd = atoi((const char*)(separate_str));
                        else {
                            strcat(temp, separate_str);
                            strcat(temp, " "); // 多分一个问题不大
                        }
                        separate_str = strtok(NULL, delim); // 后续调用
                        count++;
                    }
                    // printf("temp = %s\n", temp);
                    // printf("dstfd = %d\n", dstfd);
                    // printf("clientfd = %d\n", clientfd);
                    // printf("client[0].fd = %d\n", client[0].clientfd);
                    // printf("client[1].fd = %d\n", client[1].clientfd);
                    if(dstfd > clientNum) { // 传输失败, 发送错误信息给源客户端
                        send(clientfd, ":s fail", 7, 0);
                        printf("Send error message to source client[%d]\n\n", clientfd);
                    }
                    else {
                        if ((iret = send(client[dstfd - 1].clientfd, temp, strlen(temp), 0)) == -1) {
                            send(clientfd, ":s fail", 7, 0);
                            printf("Send error message to source client[%d]\n\n", clientfd);
                        }
                        else {
                            send(clientfd, ":s success", 10, 0); // 传输成功, 发送成功信息
                            printf("Send message to target client[%d] successfully\n", client[dstfd - 1].clientfd);
                            printf("Send successful message to source client[%d]\n\n", clientfd);
                        }
                    }
                    break;
                }
                case 't': { // 获取时间
                    printf("[INFO] Sending server time to other client...\n");
                    memset(buffer, 0, sizeof(buffer));
                    const char *wday[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
                    time_t timep;
                    struct tm *p;
                    time(&timep);
                    p = localtime(&timep); // 取得服务器当前时间
                    sprintf(buffer, ":t %d-%d-%d %s %d:%d:%d", \
                        (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, \
                        wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
                    printf("Current server time: %s\n", buffer);
                    if((iret = send(clientfd, buffer, strlen(buffer), 0)) <= 0) // 向客户端发送响应结果
                        perror("t send");
                    printf("Send message successfully\n\n");
                    break;
                }
                case 'l': { // 获取所有客户端信息， 每个客户端以回车结尾，客户端每个字段以空格区分
                    printf("[INFO] Sending client list to a client...\n");
                    char temp[MAX_NAME_LENGTH];
                    memset(temp, 0, sizeof(temp));
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, ":l ");
                    printf("%4s%10s%15s%10s\n", "ID", "NAME", "IP", "PORT");
                    for(int i = 0; i < clientNum; i++) {
                        sprintf(temp, "%4d%10s%15s%10d\n", i + 1, client[i].name, client[i].ip, client[i].port);
                        printf("%s", temp);
                        strcat(buffer, temp);
                        memset(temp, 0, sizeof(temp));
                    }
                    if((iret = send(clientfd, buffer, strlen(buffer), 0)) <= 0) // 向客户端发送响应结果
                        perror("l send");
                    printf("Send message successfully\n\n");
                    break;
                }
                case 'n': // 获取用户名
                    printf("[INFO] Sending server name to a client...\n");
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, ":n ");
                    gethostname(buffer + 3, MAX_NAME_LENGTH); // 获得当前主机名
                    printf("server name: %s\n", buffer + 3);
                    if((iret = send(clientfd, buffer, strlen(buffer), 0)) <= 0) // 向客户端发送响应结果
                        perror("n send");
                    printf("Send message successfully\n\n");
                    break;
                case 'd': // 断开连接
                    disconnect(clientfd);
                    break;
                default:
                    break;
            }
        }
        // else { // 断开连接
        //     disconnect(clientfd);
        //     break;
        // }
    }
    printf("[INFO] Thread[%lu] ends\n\n", pthread_self());
    pthread_exit(NULL); // 销毁线程
    return NULL;
}

void disconnect(int clientfd) {
    printf("[INFO] Disconnect from the client[%d]...\n", clientfd);
    int temp = 0;
    // 从登录列表中删去该用户信息
    for (int i = 0; i < clientNum; i++)
        if (clientfd == client[i].clientfd){
            temp = i;
            break;
        }
    // printf("clientNum = %d\n", clientNum);
    // printf("temp = %d\n", temp);
    // printf("clientfd = %d\n", clientfd);
    // printf("client[0].fd = %d\n", client[0].clientfd);
    // printf("client[1].fd = %d\n", client[1].clientfd);
    for (int i = temp; i < clientNum - 1; i++){
        client[i].clientfd = client[i + 1].clientfd;
        strcpy(client[i].name, client[i + 1].name);
        strcpy(client[i].ip, client[i + 1].ip);
    }
    clientNum -= 1;
    if (send(clientfd, ":d", 2, 0) == -1) {
        perror("send");
    }
    else {
        printf("Disconnect successfully\n\n");
        close(clientfd);
    }
}

void* endServer(void* ptr) {
    char temp;
    while(1) {
        scanf("%c", &temp);
        if (temp == 'e') {
            printf("[INFO] Server terminates.\n");
            exit(0);
        }
    }
    return NULL;
}