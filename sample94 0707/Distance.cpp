#include "ev3api.h"
#include "Distance.h"

static const motor_port_t
    left_motor = EV3_PORT_C,
    right_motor = EV3_PORT_B;

#define TIRE_DIAMETER 100.0  //タイヤ直径（100mm）
#define PI 3.14159265358


static float distance = 0.0;            //走行距離
static float distance4msL = 0.0;           //左タイヤの距離
static float distance4msR = 0.0;           //右タイヤの距離
static float pre_angleL, pre_angleR;    //左右モータ回転角度の過去値

// Distance::Distance():
//     leftWheel(PORT_C), rightWheel(PORT_B){
// }


void Distance::Distance_init() {
    //初期化
    distance4msR = 0.0;
    distance4msL = 0.0;

    //モータ角度の過去値に現在値を代入
    // pre_angleL = getCount(leftWheel);
    // pre_angleR = getCount(rightWheel);
    pre_angleL = ev3_motor_get_counts(left_motor);
    pre_angleR = ev3_motor_get_counts(right_motor);
}

//距離更新
void Distance::Distance_update(){
    // float cur_angleL = getCount(leftWheel);    //左モータ回転角度の現在値
    // float cur_angleR = getCount(rightWheel);   //右モータ回転角度の現在値
    float cur_angleL = ev3_motor_get_counts(left_motor); //左モータ回転角度の現在値
    float cur_angleR = ev3_motor_get_counts(right_motor);//右モータ回転角度の現在値
    float distance4ms = 0.0; 


//走行距離 = ((円周率 * タイヤの直径) / 360) * (モータ角度過去値　- モータ角度現在値)
distance4msL = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleL - pre_angleL); //左モータ距離
distance4msR = ((PI * TIRE_DIAMETER) / 360.0) * (cur_angleR - pre_angleR); //右モータ距離
distance4ms = (distance4msL + distance4msR) / 2.0; //左右タイヤの走行距離を足して割る
distance += distance4ms;
//printf("距離%d\n",distance);

 //モータの回転角度の過去値を更新
pre_angleL = cur_angleL;
pre_angleR = cur_angleR;
}




//走行距離を取得
float Distance::Distance_getDistance(){
    //printf("走行距離:%d\n", distance);
    return distance;
}

/* 右タイヤの4ms間の距離を取得 */
float Distance::Distance_getDistance4msRight(){
    return distance4msR;
}

/* 左タイヤの4ms間の距離を取得 */
float Distance::Distance_getDistance4msLeft(){
    return distance4msL;
}

