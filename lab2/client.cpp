#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(){
    //инициализация WinSock

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2,2);
    int err;
    err = WSAStartup(wVersionRequested, &wsaData);
    if(err != 0) return 0;

    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_port = htons(1024);
    add.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buf[255], b[255];
    int t = sizeof(add);
    int flag = 1;

    while(flag){
        cout << "Enter the string (or type '1' to quit): ";
        cin.getline(buf, 255, '\n');

        if (strcmp(buf, "1") == 0) {

            sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&add, t);
            cout << "Exiting the program." << endl;
            flag = 0; 
        } else{
            sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&add, t);

            int rv = recvfrom(s, b, sizeof(b), 0, (struct sockaddr*)&add, &t);

            b[rv] = '\0';
            cout << b << endl;

        }

    }

    closesocket(s);
    WSACleanup();

    return 0;

}