#include <iostream>
#include "ev3api.h"
#include "Edge.h"
#include "Calibration.h"
#include "Distance.h"
#include "log.h"
#include "SetTarget.h"

/* ライン上でエッジを切り替える */
void Edge::changeEdge(int* edge, rgb_raw_t* m_rgb_val, int* edge_flg){
    if(m_rgb_val->r < getTarget())
    {
        *edge = *edge * -1;
        *edge_flg += 1;
        // printf("黒検知   edge: %d     edge_flg: %d\n", *edge, *edge_flg);
    }
}