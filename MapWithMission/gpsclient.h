#ifndef GPSCLIENT_H
#define GPSCLIENT_H

#include <QThread>
#include <QObject>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <atomic>
#include <thread>

#define BUFF_SIZE   2048

//void connectGPSServer(int& fd, struct sockaddr_in& ssocket, bool& conn, const char* addr, int port);

class Publisher: public QObject
{
    Q_OBJECT
public:
    Publisher(){}
    ~Publisher(){}

    void publish(std::string& msg);

signals:
    void publishGPSMessage(std::string msg);

};

class GPSClient
{
public:
    GPSClient();
    ~GPSClient();

private:
    const char* strIpAddress = {"127.0.0.1"};

    const int port={8011};

    int clientsd;
    int connState;
    char buff[BUFF_SIZE];

    struct sockaddr_in server_socket;
    struct sockaddr_in client_socket;
    bool bConnected;

    std::atomic_bool _requestStop = {false};
    std::atomic_bool _stopPressed= {false};

    std::thread clientThread;

    void threadFunction();

public:
    Publisher publisher;

    bool connectServer();

    void startConnect(const int num_tries=10);

    inline void stopConnect(){
        _stopPressed=true;
    }
    inline void sendMessage(const char* message){
        int len=strlen(message);
        send(clientsd, message, len, 0);
    }
};

#endif // GPSCLIENT_H
