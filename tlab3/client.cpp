#include <winsock2.h>
#include <iostream>
#include <string>
#include <limits>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Player {
    string fullName;
    int number;
    int age;
    int height;
    int weight;

    Player() : fullName(""), number(0), age(0), height(0), weight(0) {}
    Player(const string& name, int num, int a, int h, int w)
        : fullName(name), number(num), age(a), height(h), weight(w) {}
};

int main(){
    //инициализация WinSock
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2,2);
    int err;
    err = WSAStartup(wVersionRequested, &wsaData);
    if(err != 0) return 0;

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(1280);
    dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(s, (sockaddr *)&dest_addr, sizeof(dest_addr));

    cout << "Connected to server" << endl;
    //запрос на сервер
    string request = "GET_YOUNGEST_PLAYER";
    send(s, request.c_str(), request.length(), 0);

    //ответ
    char buffer[1024];
    int bytesReceived = recv(s, buffer, sizeof(buffer) - 1, 0); 
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; 
        cout << "Youngest player info: " << buffer << endl;
    } else {
        cout << "Error receiving data from the server" << endl;
    }

    closesocket(s);
    WSACleanup();
}
