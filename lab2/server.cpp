#include <winsock2.h>
#include <iostream>
#include <algorithm> 
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    //инициализация WinSock
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    int err;
    err = WSAStartup(wVersionRequested, &wsaData);

    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0); //домен (Интернет), тип сокета, протокол (0 == выбран автоматически)

    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_port = htons(1024);
    ad.sin_addr.s_addr = INADDR_ANY; //подставляет подходящиц IP
    bind(s, (struct sockaddr*)&ad, sizeof(ad));

    char buf[255];
    int l = sizeof(ad);

    cout << "Server is running" << endl;

    
    while(true){

        int rv = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&ad, &l);

        if (strcmp(buf, "1") == 0) {
            cout << "Close" << endl;
            break;
        }

        buf[rv] = '\0';
        cout << buf << endl;

        int flag = 0;
        for (int i = 0; buf[i+1] != '\0'; i++) {
            if (buf[i] == '@') {
                flag++;
                if (flag % 2 == 0) {
                    buf[i] = '#';
                }
            }
        }
        sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&ad, l);

    }

    closesocket(s);
    WSACleanup();

    return 0;
}
