#include "pt.h"
#include "pt-sleep.h"

/**
* ADDED BY STRAWBEES
* Provide a simplied interface to Protothreads, avoid the need to repeat
*/
#define PT_POINTER_VAR(name) pt_##name##_pointer
#define PT_THREAD_VAR(name) pt_##name##_thread

#define ptDefine(name)\
	static struct pt PT_POINTER_VAR(name);\
	static PT_THREAD (PT_THREAD_VAR(name)(struct pt* __pt__));
#define ptDeclare(name) static PT_THREAD (PT_THREAD_VAR(name)(struct pt* __pt__))
#define ptInit(name) PT_INIT(&PT_POINTER_VAR(name))
#define ptBegin() PT_BEGIN(__pt__)
#define ptSleep(ms) PT_SLEEP(__pt__, ms)
#define ptWaitUntil(condition) PT_WAIT_UNTIL(__pt__, condition)
#define ptWaitWhile(condition) PT_WAIT_WHILE(__pt__, condition)
#define ptWaitThread(name) PT_WAIT_THREAD(__pt__, PT_THREAD_VAR(name)(&PT_POINTER_VAR(name)))
#define ptYield() PT_YIELD(__pt__)
#define ptYieldUntil(condition) PT_YIELD_UNTIL(__pt__, condition)
#define ptSpawn(name) PT_SPAWN(__pt__, &PT_POINTER_VAR(name), PT_THREAD_VAR(name)(&PT_POINTER_VAR(name)))
#define ptRestart() PT_RESTART(__pt__)
#define ptExit() PT_EXIT(__pt__)
#define ptEnd() PT_END(__pt__)
#define ptSchedule(name) PT_SCHEDULE(PT_THREAD_VAR(name)(&PT_POINTER_VAR(name)))
