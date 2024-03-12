
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Component {
    string supplier;
    float cost;
    string deliveryDate;

    Component(string sup, float cst, string date) : supplier(sup), cost(cst), deliveryDate(date) {}
};

vector<Component> components = {
    Component("Supplier1", 100, "2024-02-14"),
    Component("Supplier2", 200, "2024-02-15"),
    Component("Supplier3", 300, "2024-02-15"),
    Component("Supplier4", 90, "2024-02-16"),
    Component("Supplier5", 150, "2024-02-16"),
    Component("Supplier6", 250, "2024-02-17"),
    Component("Supplier7", 350, "2024-02-17")
};

HANDLE CreateThread (
        LPSECURITY_ATTRIBUTES lpThreadAttributes, //атрибуты безопасности
        DWORD dwStackSize, //размер стека начального потока
        LPTHREAD_START_ROUTINE lpStartAddress, //фукнция потока
        LPVOID lpParametr, //аргумент для нового потока
        DWORD dwCreationFlags, //создание флагов
        LPDWORD lpThreadId //указатель на айди потока для его получения
);

DWORD WINAPI ThreadFunc(LPVOID client_socket) {
    SOCKET s2 = ((SOCKET*)client_socket)[0];
    char buf[1024];
    while (recv(s2, buf, sizeof(buf), 0)) {

        int min = components[0].cost;
        for (const auto& component : components) {
            if (component.cost < min) {
                min = component.cost; 
            }
        }

        int flag = 0;

        for (int i = 0; i < components.size(); ++i) {
            if (components[i].deliveryDate == buf && components[i].cost > min) {
                string componentInfo = components[i].supplier + ", $" + to_string(components[i].cost) + ", " + components[i].deliveryDate + '\n';
                send(s2, componentInfo.c_str(), componentInfo.length(), 0);
                flag++;
            }
        }

        if(!flag){
            string nothing = "Wasn`t there that day";
            send(s2, nothing.c_str(), nothing.length(), 0);

        }
        string endSignal = "stop";
        send(s2, endSignal.c_str(), endSignal.length(), 0);

    }

    closesocket(s2);
    return 0;
}


int numcl = 0;
void print(){
    if(numcl) cout << numcl << " client connected\n";
    else cout << "No clients connected\n";
}

int main() {
    // Инициализация WinSock
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    int err;
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        cout << "WSAStartup failed: " << err << endl;
        return 1;
    }

    // Создание сокета
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Failed to create socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Привязка сокета к адресу и порту
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

    // Установка сокета в режим прослушивания
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
        numcl++;
        print();

        DWORD thID;
        DWORD dwStackSize = 0;
        DWORD dwCreationFlags = 0;
        CreateThread(nullptr, dwStackSize, ThreadFunc, &client_socket, dwCreationFlags, &thID);
    }

    // Закрытие сокета и очистка WinSock
    closesocket(s);
    WSACleanup();

    return 0;
}


//VOID ExitThread (DWORD dwExitCode);