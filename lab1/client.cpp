#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(){
    //инициализация WinSock

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2,2);
    WSAStartup(wVersionRequested, &wsaData);

    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    //connect(s, (struct sockaddr*)&peer, sizeof(peer));
    if(connect(s, (struct sockaddr*)&peer, sizeof(peer))==SOCKET_ERROR){
        cout << "ERRROOOOOOOOOOOOOOOOOR"<<endl;
        return 1;
    }

    while (true) {
        char buf[255], b[255];
        cout << "Enter the string (or type '1' to quit): ";
        cin.getline(buf, sizeof(buf));

        if (strcmp(buf, "1") == 0) {
            cout << "Exiting the program." << endl;
            break; 
        }

        // птправляем строку на сервер
        send(s, buf, strlen(buf), 0);

        // получаем ответ от сервера
        int bytes_received = recv(s, b, sizeof(b) - 1, 0);
        if (bytes_received > 0) {
            b[bytes_received] = '\0'; 
            cout << "Server response: " << b << endl;
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

