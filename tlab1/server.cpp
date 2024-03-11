#include <winsock2.h>
#include <iostream>
#include <map>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // инициализация WinSock
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2,2);
    WSAStartup(wVersionRequested, &wsaData);

    char buf[255] = {0};
    map<string, string> weather_forecast = {
        {"Monday", "Sunny"},
        {"Tuesday", "Cloudy"},
        {"Wednesday", "Rainy"},
        {"Thursday", "Snowy"},
        {"Friday", "Windy"},
        {"Saturday", "Foggy"},
        {"Sunday", "Stormy"}
    };

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == INVALID_SOCKET){
        cerr << "Error creating socket: " <<  WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // мвязывание адреса с сокетом
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(1280);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    int c = bind(s, (struct sockaddr*)&local, sizeof(local));

    if(c == SOCKET_ERROR){
        cerr << "Socket binding error: " <<  WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    // установление соединения с клиентом
    int r = listen(s, 5);

    if(r == SOCKET_ERROR){
        cerr << "Client connection error: " <<  WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout << "Server is running" << endl;

    // коннект
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

    // работа с клиентом
    while(recv(s2, buf, sizeof(buf), 0) != 0) {
        // поиск прогноза погоды 
        string day = buf;
        string forecast;
        auto it = weather_forecast.find(day);
        if (it != weather_forecast.end()) {
            forecast = it->second;
        } else {
            forecast = "Unknown";
        }

        // отправка клиенту
        send(s2, forecast.c_str(), forecast.length(), 0);
    }

    closesocket(s);
    closesocket(s2);
    WSACleanup();

    return 0;
}
