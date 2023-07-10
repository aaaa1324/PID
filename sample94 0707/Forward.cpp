#include "Forward.h"

int setForward(int speed, int scene_speed){
    static int count = 0;
    static int count_1 = 0;

    if (speed == scene_speed)
    {
        speed = scene_speed;
    }
    else if(speed > scene_speed)
    {
        while(speed > scene_speed){
            speed--;
        }
    }
    else if(speed < scene_speed)
    {
        while(speed < scene_speed){
            speed++;
        }
    }
    return speed;
}