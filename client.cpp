#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
    int client;//client是文件的描述符，这个变量接受系统调用并通过套接系统调用返回存储的值
    int portNum = 1500; // portNum用于存储接受链接的端口号
    bool isExit = false;//isExit是用于循环结束的boll变量
    int bufsize = 1024;//设置缓冲区的大小为1024
    char buffer[bufsize];//服务器从套接字中读取一个字符，链接到一个动态变量（buffer）
    char* ip = "127.0.0.1";//IP指针为127.0.0.1

    struct sockaddr_in server_addr;//sockaddr_in是一个包含互联网的结构地址 变量server_addr包含了服务器的地址

    client = socket(AF_INET, SOCK_STREAM, 0);//socket()函数创建一个新的套接字，它需要三个参数AF_INET：套接字的地址。SOCK_STREAM：套接字的类型。第三是协议论证：应该始终为0操作系统将选择最合适的协议。

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    if (client < 0)//如果套接字失败，client返回-1
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }
    cout << "\n=> Socket client has been created..." << endl;//套接字客户端已创建

    server_addr.sin_family = AF_INET;//变量server_addr包含了服务器的地址,sin_family包含地址族的代码。它应该始终设置为AF_INET套接字地址。
    server_addr.sin_port = htons(portNum);//把主机字节顺序的端口号传递到网络字节顺序的端口号中
    /* ---------- CONNECTING THE SOCKET 连接套接字 ---------- */
    /* ---------------- connect() ---------------- */

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)//connect，connect函数被客户端调用建立到服务器的连接。此处有三个参数，套接字文件描述符，想要连接的主机地址（包括端口号）此处的端口号是服务器的端口号，不是自己的端楼号，地址的大小
        //如果成功返回0，失败返回-1 server值
        cout << "=> Connection to the server port number: " << portNum << endl;//此处显示的是服务器的端口号
    cout << "=> Awaiting confirmation from the server..." << endl; //等待服务器确认
    recv(client, buffer, bufsize, 0);//服务器确认后，recv函数是从TCP连接的另一端接收数据，接收buffer数据缓冲区中字节数为buffersize的的套接字描述符到client中

    cout << "=> Connection confirmed, you are good to go...";

    cout << "\n\n=> Enter # to end the connection\n" << endl;//此处说明客户端可以先发送一个消息给客户端
    do {
        cout << "Client: ";
        do {
            cin >> buffer;//在client中输入字符到buffer中
            send(client, buffer, bufsize, 0);//send函数是向tcp连接的另一端发送数据，发送client端的套接字描述符到buffer数据缓冲区，bufsize为参数的字节数
            if (*buffer == '#') {//如果服务器按＃结束连接，该循环将会中断并关闭服务器之间的连接。
                send(client, buffer, bufsize, 0);//send函数是向tcp连接的另一端发送数据，发送client端的套接字描述符到buffer数据缓冲区，bufsize为参数的字节数
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);//当没有断开连接时执行上面do中的步骤


        cout << "Server: ";
        do {
            recv(client, buffer, bufsize, 0);//当没有结束时，recv函数是从TCP连接的另一端接收数据，接收buffer数据缓冲区中字节数为buffersize的的套接字描述符到client中
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;//如果输入#号，则退出
            }

        } while (*buffer != 42);//当没有断开连接时执行上面do中的步骤
        cout << endl;

    } while (!isExit);

    /* ---------------- CLOSE CALL ------------- */
    /* ----------------- close() --------------- */

    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);//关闭客户端
    return 0;
}
