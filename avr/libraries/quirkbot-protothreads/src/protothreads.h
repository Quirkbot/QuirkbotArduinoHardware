#include "pt.h"
#include "pt-sleep.h"

/**
* Simplifed interface
* -------------------
* Author: Strawbees
* Provides a simplied interface to Protothreads.
*/
#define PT_UTIL_GET_MACRO(_0, _1, _2, _3, NAME, ...) NAME
#define PT_UTIL_EXPAND_ARGS(...), ##__VA_ARGS__
#define PT_UTIL_POINTER_VAR(name) pt_##name##_pointer
#define PT_UTIL_THREAD_VAR(name) pt_##name##_thread
#define PT_UTIL_RECURSIVE_POINTER_VAR(name) pt_##name##_rec_pointer
#define PT_UTIL_RECURSIVE_THREAD_VAR(name) pt_##name##_rec_thread
/**
* ptDeclare
*/
#define ptDeclare(name, ...)\
	static struct pt PT_UTIL_POINTER_VAR(name);\
	static PT_THREAD (PT_UTIL_THREAD_VAR(name)(struct pt* __pt__, ##__VA_ARGS__))
/**
* ptDeclareEvent
*/
#define ptDeclareEvent(name) ptDeclare(name)
/**
* ptDeclareBlock
*/
#define ptDeclareBlock(...) PT_UTIL_GET_MACRO(_0, ##__VA_ARGS__, _ptDeclareBlock3, _, _ptDeclareBlock1, _)(__VA_ARGS__)
#define _ptDeclareBlock(name, defArgs, callArgs)\
	static struct pt PT_UTIL_POINTER_VAR(name);\
	static PT_THREAD (PT_UTIL_THREAD_VAR(name)(struct pt* __pt__ PT_UTIL_EXPAND_ARGS defArgs));\
	static struct pt PT_UTIL_RECURSIVE_POINTER_VAR(name);\
	static PT_THREAD (PT_UTIL_RECURSIVE_THREAD_VAR(name)(struct pt* __pt__ PT_UTIL_EXPAND_ARGS defArgs)){\
		PT_BEGIN(__pt__);\
		PT_SPAWN(__pt__, &PT_UTIL_POINTER_VAR(name), PT_UTIL_THREAD_VAR(name)(&PT_UTIL_POINTER_VAR(name) PT_UTIL_EXPAND_ARGS callArgs));\
		PT_END(__pt__);\
	}
#define _ptDeclareBlock1(name)                    _ptDeclareBlock(name, (), ())
#define _ptDeclareBlock3(name, defArgs, callArgs) _ptDeclareBlock(name, defArgs, callArgs)
/**
* ptDefine
*/
#define ptDefine(name, ...) static PT_THREAD (PT_UTIL_THREAD_VAR(name)(struct pt* __pt__, ##__VA_ARGS__))
/**
* ptDefineEvent
*/
#define ptDefineEvent(name) ptDefine(name)
/**
* ptDefineBlock
*/
#define ptDefineBlock(...) PT_UTIL_GET_MACRO(_0, ##__VA_ARGS__, _, _ptDefineBlock2, _ptDefineBlock1, _)(__VA_ARGS__)
#define _ptDefineBlock(name, defArgs)\
	static PT_THREAD (PT_UTIL_THREAD_VAR(name)(struct pt* __pt__ PT_UTIL_EXPAND_ARGS defArgs))
#define _ptDefineBlock1(name)          _ptDefineBlock(name, ())
#define _ptDefineBlock2(name, defArgs) _ptDefineBlock(name, defArgs)
/**
* ptInit
*/
#define ptInit(name) PT_INIT(&PT_UTIL_POINTER_VAR(name))
/**
* ptBegin
*/
#define ptBegin() PT_BEGIN(__pt__)
/**
* ptSleep
*/
#define ptSleep(ms) PT_SLEEP(__pt__, ms)
/**
* ptWaitUntil
*/
#define ptWaitUntil(condition) PT_WAIT_UNTIL(__pt__, condition)
/**
* ptWaitWhile
*/
#define ptWaitWhile(condition) PT_WAIT_WHILE(__pt__, condition)
/**
* ptWaitThread
*/
#define ptWaitThread(name, ...) PT_WAIT_THREAD(__pt__, PT_UTIL_THREAD_VAR(name)(&PT_UTIL_POINTER_VAR(name), ##__VA_ARGS__))
/**
* ptWaitThreadEvent
*/
#define ptWaitThreadEvent(name) ptWaitThread(name)
/**
* ptWaitThreadBlock
*/
#define ptWaitThreadBlock(name, ...) PT_WAIT_THREAD(__pt__, PT_UTIL_RECURSIVE_THREAD_VAR(name)(&PT_UTIL_RECURSIVE_POINTER_VAR(name), ##__VA_ARGS__))
/**
* ptYield
*/
#define ptYield() PT_YIELD(__pt__)
/**
* ptYieldUntil
*/
#define ptYieldUntil(condition) PT_YIELD_UNTIL(__pt__, condition)
/**
* ptSpawn
*/
#define ptSpawn(name, ...) PT_SPAWN(__pt__, &PT_UTIL_POINTER_VAR(name), PT_UTIL_THREAD_VAR(name)(&PT_UTIL_POINTER_VAR(name), ##__VA_ARGS__))
/**
* ptSpawnEvent
*/
#define ptSpawnEvent(name) ptSpawn(name)
/**
* ptSpawnBlock
*/
#define ptSpawnBlock(name, ...) PT_SPAWN(__pt__, &PT_UTIL_RECURSIVE_POINTER_VAR(name), PT_UTIL_RECURSIVE_THREAD_VAR(name)(&PT_UTIL_RECURSIVE_POINTER_VAR(name), ##__VA_ARGS__))
/**
* ptRestart
*/
#define ptRestart() PT_RESTART(__pt__)
/**
* ptExit
*/
#define ptExit() PT_EXIT(__pt__)
/**
* ptEnd
*/
#define ptEnd() PT_END(__pt__)
/**
* ptSchedule
*/
#define ptSchedule(name, ...) PT_SCHEDULE(PT_UTIL_THREAD_VAR(name)(&PT_UTIL_POINTER_VAR(name), ##__VA_ARGS__))
/**
* ptBeginEvent
*/
#define ptBeginEvent() ptBegin()
/**
* ptBeginBlock
*/
#define ptBeginBlock() ptBegin();ptYield()
/**
* ptEndEvent
*/
#define ptEndEvent() ptYieldUntil(false);ptEnd()
/**
* ptEndBlock
*/
#define ptEndBlock() ptExit();ptEnd()
