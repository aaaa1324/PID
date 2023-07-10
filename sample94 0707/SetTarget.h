/**
 *SetTarget.h
 */
#ifndef SetTarget_H_
#define SetTarget_H_

#include "Calibration.h"
#include "ev3api.h"
using namespace ev3api;

#define L_COURSE 0
#define R_COURSE 1

// extern FILE *bt;

class SetTarget{
    public:

    int g_white;
    int g_black;

    float target;
    int line_color;	   //どの色のライン上をトレースするか
    float getTarget(); //トレース目標値設定
    
};

#endif
