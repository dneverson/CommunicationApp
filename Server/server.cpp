/**********************************************************************
	Name		: Communication App UDP Server
	Author		: Derry Everson
	Date		: 08/16/2021
	Purpose		: 

***********************************************************************/

#include <iostream>
#include <ws2tcpip.h>

#include <iomanip>
//#pragma comment(lib, "ws2_32.lib") // g++/MinGW compilation issue, apply -lws2_32 arg


using namespace std;

const string getTimeString() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%m-%d-%Y %H:%M:o%S");
    auto str = oss.str();
    return str;
}

int main() {

    cout << "Server Start Up: " << getTimeString() << endl;

    ////////////////////////////////////////////////////////////
	// INIT WINSOCK
	////////////////////////////////////////////////////////////
    WSADATA data;
    WORD version = MAKEWORD(2, 2);

    int wsOK = WSAStartup(version, &data);
    
    if (wsOK != 0) {
        cout << "Can't start Winsock! " << wsOK;
        return -1;
    }

    ////////////////////////////////////////////////////////////
	// SOCKET CREATION AND BINDING
	////////////////////////////////////////////////////////////
    SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000); // conver from little to big endian

    if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
        cout << "Can't bind socket! " << WSAGetLastError() << endl;
        return -1;
    }


	////////////////////////////////////////////////////////////
	// MAIN LOOP SETUP AND ENTRY
	////////////////////////////////////////////////////////////
    sockaddr_in client;
    int clientLength = sizeof(client);
    int bufLength = 1024;

    char buf[bufLength];

    // enter loop
    while (true) {
        ZeroMemory(&client, clientLength);
        ZeroMemory(buf, bufLength);
        
        // wait for message
        int bytesIn = recvfrom(in, buf, bufLength, 0, (sockaddr*)&client, &clientLength);
        if (bytesIn == SOCKET_ERROR) {
            cout << "Error reciving from client " << WSAGetLastError() << endl;
            continue;
        }


        // display message and client info
        int byteSize = 256;
        char clientIP[byteSize];
        ZeroMemory(clientIP, byteSize);

        //inet_ntop(AF_INET, &client.sin_addr, clientIP, byteSize);
        inet_ntop(AF_INET, &client.sin_addr, clientIP, byteSize);

        cout << "Client: " << clientIP << ", Message: " << buf << endl;

    }
    //close socket
    closesocket(in);

    //shutdown winsock
    WSACleanup();

}
