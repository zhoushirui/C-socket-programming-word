#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//在其中已经包含定义了sockaddr_in的结构
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    int client, server;//client和server是两个文件的描述符，这两个变量接受系统调用并通过套接系统调用返回存储的值
    int portNum = 1500;//portNum用于存储接受链接的端口号
    bool isExit = false;//isExit是用于循环结束的boll变量
    int bufsize = 1024;//设置缓冲区的大小为1024
    char buffer[bufsize];//服务器从套接字中读取一个字符，链接到一个动态变量（buffer）

    struct sockaddr_in server_addr;//sockaddr_in是一个包含互联网的结构地址 变量server_addr包含了服务器的地址
    socklen_t size;//socklen_t是一个内联类型，宽度至少为32位

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    client = socket(AF_INET, SOCK_STREAM, 0);//socket()函数创建一个新的套接字，它需要三个参数AF_INET：套接字的地址。SOCK_STREAM：套接字的类型。第三是协议论证：应该始终为0操作系统将选择最合适的协议。

    if (client < 0)//如果套接字失败，client返回-1
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket server has been created..." << endl;//套接字服务器已创建

    server_addr.sin_family = AF_INET;//变量server_addr包含了服务器的地址,sin_family包含地址族的代码。它应该始终设置为AF_INET套接字地址。
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);//INADDR_ANY包含主机的ip地址，也是运行服务器的机器的永远ip地址。
    server_addr.sin_port = htons(portNum);//把主机字节顺序的端口号传递到网络字节顺序的端口号中

    /* ---------- BINDING THE SOCKET ---------- 绑定socket*/
    /* ---------------- bind() ---------------- */


    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) //bind，系统调用将套接字绑定到地址，在这种情况下是当前主机和端口号的地址服务器将在其上运行。
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }// client是套接字文件描述符，(struct sockaddr*)&server_addr是一个指到类型为sockaddr结构的指针

    size = sizeof(server_addr);//设置size大小为服务器地址的大小
    cout << "=> Looking for clients..." << endl;//寻找clients

    /* ------------- LISTENING CALL ------------- */
    /* ---------------- listen() ---------------- */

    listen(client, 1);//监听系统调用允许在套接字链接上进行监听，如果没有监听到则闲置。
    //client为套接字文字描述符，1为客户数量的大小即服务器可以连接的数量。

 /* ------------- ACCEPTING CLIENTS  ------------- 接受客户*/
    int clientCount = 1;//设置客户端的数量为1
    server = accept(client,(struct sockaddr *)&server_addr,&size);//accept（）函数调用会导致进程阻塞直到客户端连接到服务器。
    //当来自客户端的连接已经结束时函数才能成功建立。
    //客户端在此返回一个新的文件描述符，并且在此次连接上的所有通信都使用新的文件描述符。
    //(struct sockaddr *)&server_addr是另一个客户端的指针指向连接结束
    if (server < 0) //检查返回值accept（）是否有效
        cout << "=> Error on accepting..." << endl;

    while (server > 0) //当返回值有效时
    {
        strcpy(buffer, "=> Server connected...\n");//把"=> Server connected...\n"置入buffer缓冲区中
        send(server, buffer, bufsize, 0);//send函数是向tcp连接的另一端发送数据，发送server端的套接字描述符到buffer数据缓冲区，bufsize为参数的字节数
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;//#建结束连接

        cout << "Client: ";
        do {
            recv(server, buffer, bufsize, 0);//recv函数是从TCP连接的另一端接收数据，接收buffer数据缓冲区中字节数为buffersize的的套接字描述符到server端
            cout << buffer << " ";//recv接收后从buffer中输出
            if (*buffer == '#') {//如果服务器按＃结束连接，该循环将会中断并关闭服务器之间的连接。
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;//在server中输入字符到buffer中
                send(server, buffer, bufsize, 0);//send函数是向tcp连接的另一端发送数据，发送server端的套接字描述符到buffer数据缓冲区，bufsize为参数的字节数

                if (*buffer == '#') {//如果服务器按＃结束连接，该循环将会中断并关闭服务器之间的连接。
                    send(server, buffer, bufsize, 0);//send函数是向tcp连接的另一端发送数据，发送server端的套接字描述符到buffer数据缓冲区，bufsize为参数的字节数
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');//当没有断开连接时执行上面do中的步骤

            cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);//从client端接收套接字符

                cout << buffer << " ";//输出接收的套接字符
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;//如果输入#号，则退出
                }
            } while (*buffer != '*');
        } while (!isExit);

        /* ---------------- CLOSE CALL ------------- */
        /* ----------------- close() --------------- */
        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);//如果结束，则用inet_ntoa函数把从客户端获取的IP转换为数据包
        close(server);//关闭服务器
        cout << "\nGoodbye..." << endl;
        isExit = false;
        exit(1);
    }

    close(client);//关闭客户端
    return 0;
}
