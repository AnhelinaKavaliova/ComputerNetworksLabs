#include <winsock2.h>
#include <iostream>
#include <algorithm> 
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(){

    //инициализация WinSock

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2,2);
    WSAStartup(wVersionRequested, &wsaData);

    char buf[255] = {0}, res[100], b[255] = {0}, *Res;
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0); //домен (Интернет), тип сокета, протокол (0 == выбран автоматически)

    if(s == INVALID_SOCKET){
        cerr << "Error creating socket: " <<  WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    //связывание адреса с сокетом

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(1280);
    local.sin_addr.s_addr = htonl(INADDR_ANY); //хранит физический IP-адрес компьютера
    int c = bind(s, (struct sockaddr*)&local, sizeof(local));

    if(c == SOCKET_ERROR){
        cerr << "Socket binding error: " <<  WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    //установление соединения с клиентом

    int r = listen(s, 5);

    if(r == SOCKET_ERROR){
        cerr << "Client connection error: " <<  WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout << "Server is running" << endl;

    //получение запроса клиента

    struct sockaddr_in remote_addr; 
    int size = sizeof(remote_addr);
    SOCKET s2 = accept(s, (struct sockaddr*)&remote_addr, &size);

    if(s2 == INVALID_SOCKET){
        cerr << "Error accepting request: " <<  WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout << "Connection established" << endl;

    //работа с клиентом

    while(recv(s2, b, sizeof(b), 0) != 0){

        sort(b, b + strlen(b));

        Res = new char[strlen(b) + 1];
        strcpy(Res, b);
        send(s2, Res, strlen(Res), 0);
        delete[] Res;

    }

    closesocket(s);
    closesocket(s2);
    WSACleanup();

    return 0;
}
