#ifndef _DISTANCE_H
#define _DISTANCE_H
#include <stdio.h>
/* タイヤ直径 */
//#define TIRE_DIAMETER 100.0 //(91→101mm）

/* 円周率 */
#define PI 3.14159265358

class Distance
{
    public:

    //Distance();
    void Distance_init();              //初期化
    void Distance_update();             //距離の更新
    float Distance_getDistance();       //走行距離を取得
    float Distance_getDistance4msRight();  //右タイヤの距離を取得
    float Distance_getDistance4msLeft();  //左タイヤの距離を取得

};

#endif