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

class Client {
    public:
        string userName = genRandUserName(6);
        string recipientName = genRandUserName(6);
        const char* serverIp = "127.0.0.1";
        int serverPort = 54000;

    string genRandUserName(const int len) {
        string result;
        static const char alphaNum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        srand((unsigned) time(NULL) * getpid());
        result.reserve(len);
        for (int i = 0; i < len; ++i) {
            result += alphaNum[rand() % (sizeof(alphaNum) - 1)];
        }
        return result;
    }
};

const string getTimeString() {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream oss;
    oss << put_time(&tm, "%m-%d-%Y %H:%M:o%S");
    auto str = oss.str();
    return str;
}



// void getUserData(Client& object) {
//     string userName;
//     string recipientName;
//     string serverIp;
//     int serverPort;

//     cout << "Server IP [Default 127.0.0.1]: ";
//     cin >> serverIp;

//     cout << "Server Port [Default 54000]: ";
    
  


//     if (!(cin >> serverPort)) {
//         serverPort = 54000;
//         cin.clear();
//     }
    
//     cout << "User Name: ";
//     if (!(cin >> userName)) {
//         userName = genRandUserName(6);
//         cin.clear();
//     }

//     cout << "Recipient Name: ";
//     if (!(cin >> recipientName)) {
//         recipientName = genRandUserName(6);
//         cin.clear();
//     }
// }





int main( int argc, char* argv[]) {

    Client client;
    //getUserData(newUser);

    cout << client.userName << endl;

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
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(client.serverPort); // conver from little to big endian
    inet_pton(AF_INET, client.serverIp, &server.sin_addr);
    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    int exit = 0;

    while (!exit) {
        // write out to that socket
        string s;
        cout << "Msg: ";
        cin >> s;
        if (s == "!q") {
            exit = 1;
        }
        int sendOK = sendto(out, s.c_str(), s.size()+1, 0, (sockaddr*)&server, sizeof(server));

        if (sendOK == SOCKET_ERROR) {
            cout << "Failed to send! " << WSAGetLastError() << endl;
            return -1;
        }
    }




    //close socket
    closesocket(out);

    //shutdown winsock
    WSACleanup();

}
