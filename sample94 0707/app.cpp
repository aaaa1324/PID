#include "ev3api.h"
#include "app.h"
#include "Distance.h"
#include "Scene.h"
#include "Motor.h"
#include "SetTarget.h"
#include "ColorSensor.h"
#include "Forward.h"
#include "TouchSensor.h"
#include <stdio.h>
#include "Tracer.h"
#include "util.h"
#include "Clock.h"  
using namespace ev3api;

// #if defined(MAKE_SIM)
// static const int _SIM = 1;
// #elif defined(MAKE_EV3)
// static const int _EV3 = 0;
// #else
// static const int _EV3 = 0;
// #endif


SetTarget setTarget;
Calibration calibration;
Distance distance;
Tracer tracer;
Clock clock;

int edge = -1; //左エッジ(→)
float now_distance = 0.0; //走行距離
float direction; //現在の方位
float target_distance;

bool change_scene_flag = false;

int card_color = 0;

rgb_raw_t m_rgb_val; // RGB Raw値を格納する構造体 (ロボコン

signed char turn; /* 旋回命令 */


// 実機用の設定値
double P = 0.55;    /*※ 比例ゲイン 1.6 */
double I = 0.0087; /*※ 積分ゲイン 0.002 */
double D = 0.051;


/* 関数プロトタイプ宣言 */
void ts_start();
static bool sonar_alert(void);


static const motor_port_t
    left_motor = EV3_PORT_C,
    right_motor = EV3_PORT_B;
static const sensor_port_t
    color_sensor = EV3_PORT_2;





void tracer_task(intptr_t exinf) { // <1>
  tracer.run(); // <2>
//    printf("RGBaaaaaa");
  
  ext_tsk();
}

/* 
    実機を初期化
 */
void InitRealMachine(){
  

    // カラーキャリブレーションを開始
    // printf("calibration start\n");
    calibration.color();

    // PID値を設定
    // setPvalue(&P, &I, &D);
    // setIvalue(&P, &I, &D);
    // setDvalue(&P, &I, &D);

    ev3_motor_config (left_motor, LARGE_MOTOR);
    ev3_motor_config (right_motor, LARGE_MOTOR);

    tslp_tsk(125 * 1000U);

    // LEDライトのカラーをオレンジ色に設定
    // ev3_led_set_color(LED_ORANGE);

   

}


/*
    対象のマシンのタスクを開始
*/
void startTargetMachineTask()
{
    // タスク開始
    ts_start();
    printf("タスク開始\n");


    // LEDライトのカラーをオレンジ色に設定
    // ev3_led_set_color(LED_GREEN);

    // 周期ハンドラ開始
    sta_cyc(CYC_PID);
    printf("周期ハンドラ\n");

    
    // 計測器初期化
    distance.Distance_init();
    // 合計走行距離を記録するタスクスタート
    act_tsk(TOTAL_DISTANCE_TASK);
    // printf("走行距離\n");


}

/* 走行距離を計測するタスク */
void total_distance(intptr_t unused)
{
    // printf("tbotal_distance\n");
    while (1)
    {
        distance.Distance_update();
        tslp_tsk(4 * 1000U); /* 4msec周期起動 */
    }
}
/*
    対象のマシンのタスクを終了
*/
void endTargetMachineTask()
{
    // モータを停止
    ev3_motor_stop(left_motor, false);
    ev3_motor_stop(right_motor, false);
    // printf("停止\n");

    // タスク終了
    ext_tsk();

}

/* メインタスク */
void main_task(intptr_t unused)
{  
     uint32_t duration; // <2>
     // 対象マシンのタスクを開始
        startTargetMachineTask();

         tracer.init(); // <3>
        //sta_cyc(TRACER_CYC); // <4>
    // 終了ボタンを押していない間
    printf("main task\n");
    while (!ev3_button_is_pressed(LEFT_BUTTON))
    { 

        // 4msec周期起動
        tslp_tsk(100* 1000U);
        // RGB値RAWを取得(カラーセンサー)
        // ev3_color_sensor_get_rgb_raw(color_sensor, &m_rgb_val);
        // calibration.getRawColor(m_rgb_val);
        // printf("RGB:%d\n",m_rgb_val.r);
         
        
    }
      stp_cyc(TRACER_CYC); // <3>
      tracer.terminate(); // <4>
      ext_tsk(); // <5>
      
}



/* turn値が(大きすぎる・小さすぎる)場合は制限する */
int setLimitTurn(signed char turn)
{
    const int max = 100;
    const int min = -100;
    if (max < turn)
    {
        turn = max;
    }
    else if(turn < min)
    {
        turn = min;
    }
    return turn;
}

/*
    現在の走行距離がシーン変更地点に達しているかどうか
*/
bool isChangeLocation(float target_distance){
    
    if  ((now_distance >= target_distance))
    {
        // 以下の条件を満たす場合、Trueを返す。
        // 1.現在の走行距離がシーン変更地点に達している場合
        // 2.RGBが黒ライン上にある場合
        return true;
    } 
    else
    {
        return false;
    }

}



/*PIDタスク*/


int forward = 0;

void pid_task(intptr_t exinf)
  
{     uint64_t  time;
      clock.now();
//   printf("時間:%11d\n", time);
    
    //  extern int g_white;
    //  extern int g_black;
 

    
    // RGB値取得
    // printf("pid task\n");
    now_distance = distance.Distance_getDistance();
    int rgb_ave = calibration.getAveRGB();

    
        
        
    
    /* -----PID制御--------------- */
    static double zure = 0.0;                                                   /* 目標値との偏差 */
    static double zure1 = 0.0;                                                  /* ひとつ前の目標値との偏差 */
    zure1 = zure;                                                         /* 現在の目標値との偏差を更新 */
    //zure = (calibration.m_rgb_ave.r - setTarget.getTarget());
    zure = (rgb_ave - setTarget.getTarget());
    // printf("PID制御\n");
    // zure = (m_rgb_val.r - getTarget()) * ((130 - 17) / (g_white - g_black));                                         //現在の偏差を取得

    //　積分計算結果を設定
    static int m_num = 0;
    if (m_num >= 13)
    {
        m_num = 0;
    }

    static double m_i_list[13] = {};                                         // 計算用の偏差(直近13個のみ対象)
    m_i_list[m_num] = (zure1 + zure) / 2.0 * 0.004;
    m_num++;

    double sum = 0.0;
    for (int i = 0; i < 13; i++)
    {
        // 配列内の積分計算結果の合計を算出
        sum += m_i_list[i];
    }

    // シーン取得
    static  linecourse_scene current_scene;
    static bool sceneFlag = true;
    if(sceneFlag){
        // 各シーンの値を設定
        // printf("格納開始\n");
        current_scene = setLineCourseScene();
        current_scene.edge = current_scene.edge * edge;
        sceneFlag = false;
        // printf("forward:%d\n",current_scene.forward);

    }

    /* シーン更新 */
    // static int sceneIndex = 0;
    if (isChangeLocation(current_scene.target_distance) || change_scene_flag)
    {
        // 切り替え地点に到達した場合
        current_scene = setLineCourseScene();
        current_scene.edge = current_scene.edge * edge;
        change_scene_flag = false;
        // printf("シーン更新\n");
        // printf("距離：%d, 速度：%d 通過\n",(int)current_scene.target_distance,(int)current_scene.forward);
        // printf("走行距離：%d, 速度：%d,シーン：%d 通過\n",(int)now_distance,(int)forward,sceneIndex);
    }

    P = current_scene.P;
    I = current_scene.I;
    D = current_scene.D;

    static int logCount = 0;
    if (logCount >= 100){
        printf("P:%f\n",current_scene.P);
        printf("I:%f\n",current_scene.I);
        printf("D:%f\n",current_scene.D);
        printf("総走行距離:%f\n",now_distance);
        printf("RGB_AVE:%d\n",rgb_ave);
        logCount = 0 ;
    }

    logCount++;

    // printf("P:%f\n",current_scene.P);
    // printf("I:%f\n",current_scene.I);
    // printf("D:%f\n",current_scene.D);
    // printf("distance:%f\n",current_scene.target_distance);
    // printf("A:%d\n",ev3_battery_current_mA());
    // printf("V:%d\n",ev3_battery_voltage_mV());
    // ev3_battery_voltage_mV();
    // printf("総走行距離:%f\n",now_distance);
    // printf("target_distance%f\n", current_scene.target_distance);
    // char fileName1[3][13] = {"Distance.txt"};

    // printf("ズレ:%f\n",zure);
    // printf("赤平均:%d\n",calibration.m_rgb_ave.r);

    // turn値
    turn = (zure * P + sum * I + (zure - zure1) / 0.004 * D) * current_scene.edge;
    turn = setLimitTurn(turn);// turnの制限
    // 前進値
    forward = setForward(forward, current_scene.forward);

    //  printf("%d\n", turn);
    //  printf("%d\n", forward);
    
    // 2つのモーターでロボットのステアリング操作を行う
    ev3_motor_steer(left_motor,right_motor,forward,turn);
    
    ext_tsk();
}


//*****************************************************************************
// 関数名 : ts_start
// 引数 : unused
// 返り値 : なし
// 概要 : シュミレーター(_SIM): 自動で走行開始
// 　　　 実機: 本体左ボタンを押すと、走行開始する
//*****************************************************************************
void ts_start()
{
        // 実機の場合
        printf("タスク開始\n");
        while (1)
        {
            if (ev3_button_is_pressed(LEFT_BUTTON))
            {
                return;
            }
            tslp_tsk(100 * 1000U);
        }
}