#pragma once
#include <pthread.h>


#define PORT_NUMBER             4385
#define INPUT_BUFFER_SIZE       1024

#define CMD_MOTOR_SETTINGS      0x01
#define CMD_MOTION              0x02


class Server
{
public:
    static Server* getInstance();
private:
    Server();
    ~Server();
    Server(Server const&) {}
    Server& operator=(Server const&) {}
    void processReceivedData(char* buffer, int length);
    static Server* instance;

    friend void* serverTask(void* param);
    int server_socket;
    int client_socket;
    pthread_t server_thread;
};
