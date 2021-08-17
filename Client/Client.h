#ifndef CLIENT_H
#define CLIENT_H

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

#endif