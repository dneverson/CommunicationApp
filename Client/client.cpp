/**********************************************************************
	Name		: Communication App UDP Server
	Author		: Derry Everson
	Date		: 08/16/2021
	Purpose		: 

***********************************************************************/

#include <iostream>
#include <ws2tcpip.h>

#include <atomic>
#include <thread>

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


class Client {
    private:
        string       userID;
        string       recvID;
        const char*  serverIp;
        int          serverPort;

    public:
        atomic<bool> stop;
        thread       incoming;
        thread       outgoing;
        SOCKET       in;
        SOCKET       out;
        sockaddr_in  server;
        
        Client(string UID, string RID, const char* SIP, int SPO) {
            userID     = !UID.empty()?UID:genUserID(6,1);
            recvID     = !RID.empty()?RID:genUserID(6,2);
            serverIp   = SIP!=""?SIP:"127.0.0.1";
            serverPort = SPO!=0?SPO:54000;
            stop       = false;
        }

        ////////////////////////////////////////////////////////////
        // GETTERS
        ////////////////////////////////////////////////////////////
        string getUserID() { return userID; }
        string getRecvID() { return recvID; }
        const char* getServerIp() { return serverIp; }
        int getServerPort() { return serverPort; }

        ////////////////////////////////////////////////////////////
        // SETTERS
        ////////////////////////////////////////////////////////////
        void setUserID(string input) { userID = input; }
        void setRecvID(string input) { recvID = input; }
        void setServerIp(const char* input) { serverIp = input; }
        void setServerPort(int input) { serverPort = input; }

        ////////////////////////////////////////////////////////////
        //FUNCTIONS
        ////////////////////////////////////////////////////////////

        /////// TBD DESCRIPTION ///////
        string genUserID(const int len, const int c) {
            string result;
            static const char alphaNum[63] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
            srand((unsigned) time(NULL)+c * getpid());
            result.reserve(len);
            for (int i = 0; i < len; ++i) {
                result += alphaNum[rand() % (sizeof(alphaNum) - 1)];
            }
            return result;
        }

        /////// TBD DESCRIPTION ///////
        void openWinsock() {
            WSADATA data;
            WORD version = MAKEWORD(2, 2);
            int wsOK = WSAStartup(version, &data);
            if (wsOK != 0) outputError("Can't start Winsock! ", wsOK);
        }

        /////// TBD DESCRIPTION ///////
        void closeWinsock() {
            WSACleanup();
        }

        /////// TBD DESCRIPTION ///////
        void startSocket() {
            in  = socket(AF_INET, SOCK_DGRAM, 0);
            out = socket(AF_INET, SOCK_DGRAM, 0);
            server.sin_family = AF_INET;
            server.sin_port = htons(serverPort); // conver from little to big endian
            inet_pton(AF_INET, serverIp, &server.sin_addr);
        }

        /////// TBD DESCRIPTION ///////
        void closeSocket(){
            closesocket(in);
            closesocket(out);
        }

        /////// TBD DESCRIPTION ///////
        void send(string str) {
            int sendOK = sendto(out, str.c_str(), str.size()+1, 0, (sockaddr*)&server, sizeof(server));
            if (sendOK == SOCKET_ERROR) outputError("Failed to send! ", WSAGetLastError());
        }

        /////// TBD DESCRIPTION ///////
        void recieve() {

        }

        /////// TBD DESCRIPTION ///////
        void outputError(string msg, int err) {
            cout << msg << err << endl;
        }


};

int main( int argc, char* argv[]) {
    Client client("","","",0);

    //DEBUGING REMOVE LATER
    cout << client.getUserID() << " " << client.getRecvID() << " " << client.getServerIp() << " " << client.getServerPort() << endl;

    client.openWinsock();
    client.startSocket();

    int exit = 0;

    while (!exit) {
        // write out to that socket
        string str;
        cout << client.getUserID() << ": ";
        cin >> str;
        if (str == "!q") {
            exit = 1;
        }
        client.send(str);
    }
    client.closeSocket();
    client.closeWinsock();

}
