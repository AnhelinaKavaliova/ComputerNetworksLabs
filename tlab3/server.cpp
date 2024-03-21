#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Player {
    string fullName;
    int number;
    int age;
    int height;
    int weight;

    Player(const string& name, int num, int a, int h, int w)
        : fullName(name), number(num), age(a), height(h), weight(w) {}
};

vector<Player> players = {
    Player("Player1", 1, 20, 180, 70),
    Player("Player2", 2, 22, 175, 75),
    Player("Player3", 3, 19, 185, 80),
    Player("Player4", 4, 21, 172, 68),
    Player("Player5", 5, 20, 178, 72)
};

Player findYoungestPlayer(const vector<Player>& players) {
    Player youngest = players[0];
    for (const auto& player : players) {
        if (player.age < youngest.age) {
            youngest = player;
        }
    }
    return youngest;
}

DWORD WINAPI ThreadFunc(LPVOID client_socket) {
    SOCKET s2 = *((SOCKET*)client_socket);
    Player youngestPlayer = findYoungestPlayer(players);
    string playerInfo = youngestPlayer.fullName + ", " + to_string(youngestPlayer.number)
                        + ", " + to_string(youngestPlayer.age) + " years old, "
                        + to_string(youngestPlayer.height) + " cm, "
                        + to_string(youngestPlayer.weight) + " kg";

    //отправка клиенту
    send(s2, playerInfo.c_str(), playerInfo.length(), 0);

    closesocket(s2);
    return 0;
}

int main() {
    //инициализация WinSock
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    int err;
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        cout << "WSAStartup failed: " << err << endl;
        return 1;
    }

    //создание сокета
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Failed to create socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    //привязка сокета к адресу и порту
    sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(1280);
    local.sin_addr.s_addr = INADDR_ANY;
    if (bind(s, (sockaddr*)&local, sizeof(local)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    //режим прослушивания
    if (listen(s, 5) == SOCKET_ERROR) {
        cout << "Listen failed: " << WSAGetLastError() << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout << "Server receive ready" << endl;

    SOCKET client_socket;
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    while (true) {
        cout << "Waiting for incoming connections..." << endl;
        client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == INVALID_SOCKET) {
            cout << "Accept failed: " << WSAGetLastError() << endl;
            closesocket(s);
            WSACleanup();
            return 1;
        }

        cout << "Client connected" << endl;

        DWORD thID;
        DWORD dwStackSize = 0;
        DWORD dwCreationFlags = 0;
        CreateThread(nullptr, dwStackSize, ThreadFunc, &client_socket, dwCreationFlags, &thID);
    }

    closesocket(s);
    WSACleanup();

    return 0;
}

