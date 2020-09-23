#include "pt.h"
#include "pt-sleep.h"

/**
* Simplifed interface
* -------------------
* Author: Strawbees
* Provides a simplied interface to Protothreads.
*/
#define PT_POINTER_VAR(name) pt_##name##_pointer

#define PT_THREAD_VAR(name) pt_##name##_thread

#define PT_RECURSIVE_POINTER_VAR(name) pt_##name##_rec_pointer

#define PT_RECURSIVE_THREAD_VAR(name) pt_##name##_rec_thread

#define ptDeclare(name, ...)\
	static struct pt PT_POINTER_VAR(name);\
	static PT_THREAD (PT_THREAD_VAR(name)(struct pt* __pt__, ##__VA_ARGS__))

#define ptDeclareEvent ptDeclare

#define ptDeclareBlock(name, ...) ptDeclare(name, ##__VA_ARGS__);\
	static struct pt PT_RECURSIVE_POINTER_VAR(name);\
	static PT_THREAD (PT_RECURSIVE_THREAD_VAR(name)(struct pt* __pt__, ##__VA_ARGS__)){\
		PT_BEGIN(__pt__);\
		PT_SPAWN(__pt__, &PT_POINTER_VAR(name), PT_THREAD_VAR(name)(&PT_POINTER_VAR(name), ##__VA_ARGS__));\
		PT_END(__pt__);\
	}


#define ptDefine(name, ...) static PT_THREAD (PT_THREAD_VAR(name)(struct pt* __pt__, ##__VA_ARGS__))

#define ptDefineEvent ptDefine

#define ptDefineBlock ptDefine

#define ptInit(name) PT_INIT(&PT_POINTER_VAR(name))

#define ptBegin() PT_BEGIN(__pt__)

#define ptSleep(ms) PT_SLEEP(__pt__, ms)

#define ptWaitUntil(condition) PT_WAIT_UNTIL(__pt__, condition)

#define ptWaitWhile(condition) PT_WAIT_WHILE(__pt__, condition)

#define ptWaitThread(name, ...) PT_WAIT_THREAD(__pt__, PT_THREAD_VAR(name)(&PT_POINTER_VAR(name), ##__VA_ARGS__))

#define ptWaitThreadEvent ptWaitThread

#define ptWaitThreadBlock(name, ...) PT_WAIT_THREAD(__pt__, PT_RECURSIVE_THREAD_VAR(name)(&PT_RECURSIVE_POINTER_VAR(name), ##__VA_ARGS__))

#define ptYield() PT_YIELD(__pt__)

#define ptYieldUntil(condition) PT_YIELD_UNTIL(__pt__, condition)

#define ptSpawn(name, ...) PT_SPAWN(__pt__, &PT_POINTER_VAR(name), PT_THREAD_VAR(name)(&PT_POINTER_VAR(name), ##__VA_ARGS__))

#define ptSpawnEvent ptSpawn

#define ptSpawnBlock(name, ...) PT_SPAWN(__pt__, &PT_RECURSIVE_POINTER_VAR(name), PT_RECURSIVE_THREAD_VAR(name)(&PT_RECURSIVE_POINTER_VAR(name), ##__VA_ARGS__))

#define ptRestart() PT_RESTART(__pt__)

#define ptExit() PT_EXIT(__pt__)

#define ptEnd() PT_END(__pt__)

#define ptSchedule(name, ...) PT_SCHEDULE(PT_THREAD_VAR(name)(&PT_POINTER_VAR(name), ##__VA_ARGS__))

#define ptBeginEvent() ptBegin()

#define ptBeginBlock() ptBegin();ptYield()

#define ptEndEvent() ptYieldUntil(false);ptEnd()

#define ptEndBlock() ptExit();ptEnd()
