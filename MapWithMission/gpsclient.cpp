#include "gpsclient.h"
#include <QMetaType>

GPSClient::GPSClient()
{
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::string>("std::string&");
}

GPSClient::~GPSClient(){
    if(bConnected){
        close(clientsd);
    }
}

bool GPSClient::connectServer(){
    clientsd=socket(AF_INET, SOCK_STREAM, 0);
    if(clientsd==-1){
        std::cout<<"Socket error"<<std::endl;
        return false;
    }
    server_socket.sin_family=AF_INET;
    server_socket.sin_port=htons(port);
    server_socket.sin_addr.s_addr=inet_addr(strIpAddress);
    if(connect(clientsd, (struct sockaddr*)(&server_socket), sizeof(server_socket))<0){
        std::cout<<"Connect error"<<std::endl;
        bConnected=false;
        close(clientsd);
        return false;
    }
    bConnected=true;
    std::cout<<"server connect successfully"<<std::endl;
    return true;
}

void GPSClient::startConnect(const int num_tries){
    _requestStop=false;
    int i=0;
    for(;i<num_tries;++i){
        if(!connectServer()){
            usleep(30e3);
            continue;
        }
        break;
    }
    if(bConnected){
        clientThread=std::thread(&GPSClient::threadFunction, this);
        clientThread.detach();  //detach is very important, or the thread will not terminate even if threadFunction is over
    }
    else{
        std::cout<<"Cannot connect to GPSServer, give up!"<<std::endl;
    }
}

void GPSClient::threadFunction(){
    while(!_requestStop){
        if(!_stopPressed)
            sendMessage("gps");
        else
            sendMessage("stop");
        int rn=recv(clientsd, buff, sizeof(buff), 0);
        buff[rn]='\0';
        if(strcmp(buff,"nogps")==0){
            continue;
        }
        else if(strcmp(buff, "stop")==0){
            //std::cout<<"Stop received"<<std::endl;
            _requestStop=true;
            bConnected=false;
            _stopPressed=false;
            break;
        }
        else{
            std::string message(buff);
            std::cout<<message<<std::endl;
            publisher.publish(message);
        }
    }
}

void Publisher::publish(std::string &msg){
    emit publishGPSMessage(msg);
}
/*

void connectGPSServer(int& fd, struct sockaddr_in& ssocket, bool& conn, const char* addr, int port){
    fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd==-1){
        std::cout<<"Socket error"<<std::endl;
        return;
    }
    ssocket.sin_family=AF_INET;
    ssocket.sin_port=htons(port);
    ssocket.sin_addr.s_addr=inet_addr(addr);
    if(connect(fd, (struct sockaddr*)(&ssocket), sizeof(ssocket))<0){
        std::cout<<"Connect error"<<std::endl;
        conn=false;
        close(fd);
        return;
    }
    conn=true;
    std::cout<<"server connect successfully"<<std::endl;
}

*/

