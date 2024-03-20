#include <winsock2.h>
#include <iostream>
#include <algorithm>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // Инициализация WinSock
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    int err;
    err = WSAStartup(wVersionRequested, &wsaData);

    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_port = htons(1024);
    ad.sin_addr.s_addr = INADDR_ANY;
    bind(s, (struct sockaddr*)&ad, sizeof(ad));

    char buf[255];
    int l = sizeof(ad);

    cout << "Server is running" << endl;

    while (true) {
        int rv = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&ad, &l);

        if (strcmp(buf, "1") == 0) {
            cout << "Close" << endl;
            break;
        }

        buf[rv] = '\0';
        cout << "Received: " << buf << endl;

        int flag = 1;
        int removed_digits = 0;
        if (strlen(buf) > 15) {
            int len = strlen(buf);
            for (int i = 0; i < len; ++i) {
                if (isdigit(buf[i])) {
                    ++removed_digits;
                    for (int j = i; j < len; ++j) {
                        buf[j] = buf[j + 1];
                    }
                    --i; // Чтобы проверить символ на этой же позиции заново
                }
            }
            buf[len - removed_digits] = '\0'; // Установка новой конечной строки
            cout << "Removed digits: " << removed_digits << endl;
        } else {
            cout << "String length is not greater than 15. No digits removed." << endl;
            flag = 0;
        }

        sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&ad, l);

        string digits_str;
        if(flag){
            digits_str = to_string(removed_digits);
        } else {
            digits_str = to_string(flag);
        }
        sendto(s, digits_str.c_str(), digits_str.length(), 0, (struct sockaddr*)&ad, l);


    }

    closesocket(s);
    WSACleanup();

    return 0;
}
