#include <winsock2.h>
#include <iostream>
#include <string>
#include <limits>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Component {
    string supplier;
    float cost;
    string deliveryDate;

    Component() : supplier(""), cost(0.0f), deliveryDate("") {};
    Component(string sup, float cst, string date) : supplier(sup), cost(cst), deliveryDate(date) {}
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

    int flag = 1;

    while(flag){
        cout << "Enter delivery date (YYYY-MM-DD): ";
        string deliveryDate;
        cin >> deliveryDate;

        send(s, deliveryDate.c_str(), deliveryDate.length(), 0);

        char buffer[1024];
        while (true) {
            int bytesReceived = recv(s, buffer, sizeof(buffer) - 1, 0); 
            cout << "LOOk: " << bytesReceived << endl;
            if (bytesReceived <= 0) {
                break;
            }

            cout << "im herereeee\n";
            
            buffer[bytesReceived] = '\0'; 

            if (strstr(buffer, "stop") != nullptr) {
                if(bytesReceived > 5){
                    cout << "Received: ";
                    for(int i = 0; i < bytesReceived - 5; i++){
                        cout << buffer[i];
                    }
                    cout << endl;
                }
                break;
            }   


            cout << "Received: " << buffer << endl;
        }

        cout << "Do you want to continue? (0 - no, 1 - yes): ";
        cin >> flag;
        if (flag == 0) cout << "END"; 


    }

    closesocket(s);
    WSACleanup();
}
