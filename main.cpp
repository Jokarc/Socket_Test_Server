#include <iostream>
#include "User.h"
#include "Customer.h"
#include "Merchant.h"
#include "Process.h"
using namespace std;
SOCKET cSock;
SOCKET sock;
int main() {

    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);
    // 1 建立一个socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM 面向流的TCP协议

    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888); // host to net unsigned short
    sin.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("127.0.0.1");	// 如果是在内网就可以用本机的地址，这样可以防止外网访问

    if (SOCKET_ERROR == bind(sock, (sockaddr*)&sin, sizeof(sin))) // 绑定有可能不成功所有要判断
    {
        cout << "bind..error" << endl;
        return 0;
    }

    cout << "bind...access" << endl;
    // 3 listen 监听网络端口
    if (SOCKET_ERROR == listen(sock, 5))
    {
        cout << "listen..error" << endl;
        return 0;
    }
    cout << "listen...access" << endl;

    // 4 accept 等待接受客户端连接
    sockaddr_in cSin = {};
    int nAddrLen = sizeof(cSin);
    cSock = INVALID_SOCKET;
    cSock = accept(sock, (sockaddr*)&cSin, &nAddrLen);
    if (INVALID_SOCKET == cSock)
    {
        cout << "accept...error" << endl;
        return 0;
    }
    cout << "accept...access : ip = " << inet_ntoa(cSin.sin_addr) << endl;
    // ------------- server 修改如下 ----------------
    // 连接成功之后使用 whlie 持续接收客户端数据
    int Logged = 0, op = 0;
    Process process1;
    User *CurrentUser = NULL;
    Customer CurrentCustomer;
    Merchant CurrentMerchant;
    process1.work(cSock, CurrentUser, Logged, CurrentCustomer, CurrentMerchant);
    // --------------------------------------
    // 6 关闭套接字
    closesocket(sock);
    WSACleanup();

    getchar();

    delete CurrentUser;
    return 0;
}
