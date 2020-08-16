#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include "Server.h"
#include "MotionHandler.h"
#include "MotorController.h"


void* serverTask(void* param);
Server* Server::instance = nullptr;


Server::Server() {
    struct sockaddr_in serv_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("ERROR opening socket\n");
        return;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT_NUMBER);

    if (bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR on binding\n");
        return;
    }

    pthread_create(&server_thread, NULL, serverTask, NULL);
}


void* serverTask(void* param) {
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    static char buffer[INPUT_BUFFER_SIZE];
    static_cast<void>(param);

    listen(Server::instance->server_socket, 1);

    while (true) {
        printf("Waiting for client...\n");

        Server::instance->client_socket = accept(Server::instance->server_socket,
            (struct sockaddr*)&cli_addr, &clilen);

        if (Server::instance->client_socket < 0) {
            printf("ERROR on accept\n");
            continue;
        }

        printf("Client connected\n");

        while (true) {
            bzero(buffer, INPUT_BUFFER_SIZE);
            int n = read(Server::instance->client_socket, buffer, INPUT_BUFFER_SIZE);
            if (n < 0) {
                printf("ERROR reading from socket\n");
                close(Server::instance->client_socket);
                break;
            }

            Server::instance->processReceivedData(buffer, n);
        }
    }

    return nullptr;
}


Server* Server::getInstance() {
    if (instance == nullptr) {
        instance = new Server();
    }
    return instance;
}


Server::~Server() {
    close(client_socket);
    close(server_socket);
}


void Server::Destroy() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}


void Server::processReceivedData(char* buffer, int length) {
    if (length <= 0) return;

    printf("Processing data\n");

    switch (buffer[0]) {
    case CMD_MOTOR_SETTINGS:
    {
        MotorController* motor = motionHandler->getMotor(buffer[1]);

        if (motor == nullptr) return;

        RotateDirection direction = static_cast<RotateDirection>(buffer[2]);
        int power = *(reinterpret_cast<int*>(buffer + 3));

        motor->run(direction, power);
    }
    break;
    case CMD_MOTION:
    {
        int radius = *(reinterpret_cast<int*>(buffer + 1));
        int power = *(reinterpret_cast<int*>(buffer + 5));

        motionHandler->setWorkingPower(power);
        motionHandler->addCurve({ 40, radius });
        //motionHandler->setMovement(radius, power);
    }
    break;
    default:
        break;
    }
}
