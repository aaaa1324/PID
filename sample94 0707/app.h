#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

#define MAIN_PRIORITY 5                   /* メインタスクの優先度 */
#define TRACER_PRIORITY TMIN_APP_TPRI + 4 /* HIGH_PRIORITYより高くすること */

#define HIGH_PRIORITY 9 /* 並行実行されるタスクの優先度 */
#define MID_PRIORITY 10
#define LOW_PRIORITY 11

#ifndef STACK_SIZE
#define STACK_SIZE      4096
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void total_distance(intptr_t exinf);
extern void pid_task(intptr_t exinf);
extern void tracer_task(intptr_t exinf);
extern void tracer_cyc(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif