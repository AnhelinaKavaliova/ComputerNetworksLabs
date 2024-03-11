#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // инициализация WinSock
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2,2);
    WSAStartup(wVersionRequested, &wsaData);

    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == INVALID_SOCKET){
        cerr << "Error creating socket: " <<  WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    if(connect(s, (struct sockaddr*)&peer, sizeof(peer))==SOCKET_ERROR){
        cerr << "Connection error" << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    while (true) {
        char buf[255];
        cout << "Enter the day of the week (like Monday) or type '1' to quit: ";
        cin.getline(buf, sizeof(buf));

        if (strcmp(buf, "1") == 0) {
            cout << "Exiting the program." << endl;
            break; 
        }

        // отправка дня недели серверу
        send(s, buf, strlen(buf), 0);

        // получение ответа 
        char response[255];
        int bytes_received = recv(s, response, sizeof(response) - 1, 0);
        if (bytes_received > 0) {
            response[bytes_received] = '\0'; 
            cout << "Weather forecast: " << response << endl;
        } else if (bytes_received == 0) {
            cout << "The server closed the connection." << endl;
            break; 
        } else {
            cerr << "Error while receiving data from the server: " <<  WSAGetLastError() << endl;
            break; 
        }
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
