/**
 * SetTarget.c
 * 目標値算出プログラム
 */

#include "SetTarget.h"

/**
 * シミュレータかどうかの定数を定義します
 */
//  #if defined(MAKE_SIM)
// static const int _SIM = 1;
// #elif defined(MAKE_EV3)
// static const int _EV3 = 0;
// #else
// static const int _EV3= 0;
// #endif



// extern FILE *bt;
extern int g_white;
extern int g_black;

/* 目標値を算出する(カラーモード) */
float SetTarget::getTarget()
{
    
        g_white = 180;
        g_black = 0;
    
    target = (g_white + g_black) / 2.0; // TODO: 2で割ってよいか（調査中）

    return target;
}