#include <iostream>
#include <chrono>
#include <thread>
#include <wiringPi.h>
#include "MotorController.h"
#include "MotionHandler.h"
#include "configuration.h"
#include "Server.h"


using namespace std;
MotionHandler* motionHandler;


int main(/*int argc, char** argv*/)
{
    wiringPiSetup();

    motionHandler = new MotionHandler();

    Server* server = Server::getInstance();

    //RotateDirection direction = ROTATE_FORWARD;

    int radius, duration;

    while (true)
    {
        printf("radius: ");
        scanf("%d", &radius);
        if (radius == 0)
        {
            radius = INFINITE_RADIUS;
        }
	if (radius == -1)
	{
	    break;
	}

        printf("duration: ");
        scanf("%d", &duration);

        motionHandler->addCurve({ duration , radius });
        this_thread::sleep_for(chrono::milliseconds(2));
    }
    server->Destroy();

    return 0;
}
