#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;

int getUserInput();             // 处理用户输入
void Disconnect_Func(int fd);   // 断开连接
int ConnectFunc();              // 连接服务端
void *threadHandle(void* fd);   // 处理来自服务端消息的线程
void Help();                    // 输出提示信息

#define MAX_DATA 65536          // 接收到的数据最大程度
#define MAX_LEN_NAME 17         // 客户端名字的最大长度

int sockfd;                     // 客户端fd
struct sockaddr_in serverAddr;  // 服务器套接字的地址形式
struct hostent* h;              // 服务器端地址信息
pthread_t pid;                  // 线程id
int isConnected = 0;            // 是否已连接
int main(int argc, char* argv[]) {
    // 输出Help信息
    Help();
    // 接受用户请求
    while (getUserInput());
    cout << "Bye." << endl;

    return 0;
}
// 接受用户请求
int getUserInput() {
    int ret = 1; // 返回值，exit命令时返回0
    char cmd[256];   // 保存输入命令
    memset(cmd, 0, sizeof(cmd));
    // cout << "[INFO] Waiting for new command..." << endl;
    scanf("%s", cmd);
    if(cmd[0] == ':') {
        switch (cmd[1])
        {
        case 'c':// 连接
            if(isConnected) {
                cout << "Failed. You have already connected!\n\n";
                cout << "[INFO] Waiting for new command..." << endl;
            }
            else
                ConnectFunc(); // 正常建立连接 
            break;
        case 'd': // 断开连接
            Disconnect_Func(sockfd);
            break;
        case 't': // 获取时间
            if (isConnected)
                send(sockfd, "t", 1, 0);
            else {
                cout << "Failed. You have not connected!\n\n";
                cout << "[INFO] Waiting for new command..." << endl;
            }
            break;
        case 'n': // 获取服务器name
            if (isConnected)
                send(sockfd, "n", 1, 0);
            else {
                cout << "Failed. You have not connected!\n\n";
                cout << "[INFO] Waiting for new command..." << endl;
            }
            break;
        case 'l': // 获取所有已连接客户端信息
            if (isConnected)
                send(sockfd, "l", 1, 0);
            else {
                cout << "Failed. You have not connected!\n\n";
                cout << "[INFO] Waiting for new command..." << endl;
            }
            break;
        case 's': // 输入要发送的信息
            if (isConnected) {
                char buffer[MAX_DATA];
                memset(buffer, 0, sizeof(buffer));
                char des[12];
                char mes[MAX_DATA - 14];
                cout << "Please input client ID:" << endl;
                cin >> des;
                cout << "Please input the message:" << endl;
                cin >> mes;
                // getchar();
                cout << endl;
                //信息整合到buffer中，并发给服务端
                sprintf(buffer,"s ");
                strcat(buffer, des);
                strcat(buffer, " ");
                strcat(buffer, mes);
                send(sockfd, buffer, strlen(buffer), 0);
            }
            else {
                cout << "Failed. You have not connected!" << endl << endl;
                cout << "[INFO] Waiting for new command..." << endl;
            }
            break;
        case 'e':
            if (isConnected)
                Disconnect_Func(sockfd);
            ret = 0;
            break;
        case 'h':
            Help();
            break;
        default:
            cout << "Wrong command" << endl << endl;
            cout << "[INFO] Waiting for new command..." << endl;
            break;
        }
    }
    else {
        cout << "Wrong command" << endl << endl;
        cout << "[INFO] Waiting for new command..." << endl;
    }
    return ret;
}
//断开连接
void Disconnect_Func(int fd) {
    if (isConnected) {
        if (send(sockfd, "d", 1, 0) == -1)
            perror("send");
        isConnected = 0;
    }
    else {
        cout << "You have not connected to any server.\n\n"; 
        cout << "[INFO] Waiting for new command..." << endl;
    }
}
// 连接服务端
int ConnectFunc() {
    // 创建一个socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "socket() failed! code:" << errno << endl << endl;
        cout << "[INFO] Waiting for new command..." << endl;
		return -1;
	}	
	// 设置服务器端的地址信息：
    char ip[MAX_LEN_NAME];  // 服务器ip
    int port;               // 服务器端口号
    cout << "[INFO] Please input server ip: ";
    cin >> ip;
    cout << "[INFO] Please input server port: ";
    cin >> port;
    cout << endl;
    if((h = gethostbyname(ip)) == 0) { // 指定服务端的ip地址 
        printf("gethostbyname failed.\n\n");
        close(sockfd);
        cout << "[INFO] Waiting for new command..." << endl;
        return -1;
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // 指定服务端的通信端口
    memcpy(&serverAddr.sin_addr, h->h_addr, h->h_length);

    // 存储\接收数据
    if(connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) { // 向服务端发起连接请求
        cout << "Connecting failed" << endl << endl;
        close(sockfd);
        cout << "[INFO] Waiting for new command..." << endl;
        return -1;
    }
    printf("[INFO] Client has connected to server(%s:%d)!\n", ip, port);
    isConnected = 1;
    // 输入用户名
    cout << "[INFO] Please input a username:  [no longer than 20]" << endl;
    char username[MAX_LEN_NAME];
    memset(username, 0, sizeof(username));
    cin >> username;
    int iret;
    if((iret = send(sockfd, username, strlen(username), 0)) <= 0) //发送用户名
        perror("send");

    //输出接收结果 hello
    char buffer[MAX_DATA];
    recv(sockfd, buffer, MAX_DATA, 0);
    cout << "[INFO] Received contents from server: " << buffer;
    // getchar();
    cout << endl << endl;
    cout << "[INFO] Waiting for new command..." << endl;
    //新建线程：执行的函数threadHandle，实参：sockfd 
    pthread_create(&pid, nullptr, threadHandle, &sockfd);
    return 0;
}

//处理来自服务端消息的线程
void *threadHandle(void* fd) {
    int i;
    int socket_h = *(int *)fd;
    while(1) { //接收循环
        char buffer[MAX_DATA] = {0};
        if(recv(socket_h, buffer, MAX_DATA, 0) > 0) {
            //处理服务器发来的包
            printf("[INFO] Received message.\n");
            char* ptr = buffer;
            char rec[MAX_DATA]; 
            memset(rec, 0, MAX_DATA);
            while(strlen(ptr) && ptr[0] == ':') {
                // 收到服务器发来的信息并输出；每一次请求的返回结果用:t/:n/:l/:s区分
                for(i = 3; i < strlen(ptr); i++) 
                    rec[i - 3] = ptr[i];
                switch (ptr[1]) {
                case 't': // 获取时间 输出序列 :t time:t time
                    cout << "Server time: " << endl << rec << endl << endl;
                    break;
                case 'n': //获取服务端主机名 :n servername
                    cout << "Server name: " << endl << rec << endl << endl;
                    break;
                case 'l': // 获取客户端列表 :l list
                    cout << "client list: " << endl;
                    printf("%4s%10s%15s%10s\n", "ID", "NAME", "IP", "PORT");
                    cout << rec << endl;
                    break;
                case 's': // 获取信息 :s message 
                    cout << "You received a message: " << endl << rec << endl << endl;
                    break;
                case 'd': // 同意断开连接 :d
                    cout << "Server agrees to end connection" << endl << endl;
                    cout << "[INFO] Waiting for new command..." << endl;
                    pthread_exit(NULL); // 结束该线程
                    // getchar();
                    break;
                default:
                    break;
                }
                ptr = ptr + i;
            }
        }
        else {
            printf("Wrong message\n");
            break;
        }
        cout << "[INFO] Waiting for new command..." << endl;
    }
    return NULL;
}
// 输出提示信息
void Help() {
    puts("Please input a command:\n"
		"+---------------------------------+--------------------------------+\n"
		"|            Command              |            Function            |\n"
		"+---------------------------------+--------------------------------+\n"
		"|   :c                            |      Connect to a server.      |\n"
		"|   :d                            |      Disconnect.               |\n"
		"|   :t                            |      Get the server time.      |\n"
		"|   :n                            |      Get the server name.      |\n"
		"|   :l                            |      Get the client list.      |\n"
		"|   :s [CLIENT NUM] [MESSAGE]     |      Send a message.           |\n"
		"|   :e                            |      Exit.                     |\n"
        "|   :h                            |      Get help info.            |\n"
		"+---------------------------------+--------------------------------+\n");
    cout << "[INFO] Waiting for new command..." << endl;
}