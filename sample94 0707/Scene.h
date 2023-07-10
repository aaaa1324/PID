
#include <stdbool.h>


/* ラインコースシーン用の構造体 */
typedef struct {
    int forward;
    int edge;
    double P;
    double I;
    double D;
    float target_distance;
} linecourse_scene;

linecourse_scene setLineCourseScene();

