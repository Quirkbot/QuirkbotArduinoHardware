#include "pt.h"
#include "pt-sleep.h"

/**
* Simplifed interface
* -------------------
* Author: Strawbees
* Provides a simplied interface to Protothreads.
* Example:
* ------------------------------------------------------------------------------
* #include "Quirkbot.h"
* registerEvent(myEvent);
* registerBlock(myBlock, 1);
* THREAD(myEvent){
*   eventBegin();
*   spawnBlock(myBlock, 5);
*   eventEnd();
* }
* THREAD(myBlock){
*   blockBegin();
*   sleep(myBlock_args[0]);
*   while(true){
*     yield();
*   }
*   blockEnd();
* }
* void setup() {
*   initEvent(myEvent);
* }
* void loop() {
*   scheduleEvent(myEvent);
* }
**/
#define UTIL_GET_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_15,_16,\
	_17,_18,NAME,...) NAME

#define registerEvent(name)\
	PT_THREAD(name##_starts(struct pt *));\
	struct pt name##_pointer

#define registerBlock(name, num_args)\
	float name##_args[num_args];\
	PT_THREAD(name(struct pt *, float[num_args]));\
	struct pt name##_pointer;\
	PT_THREAD(name##_rec (struct pt * _pt_)){\
		PT_BEGIN(_pt_);\
		PT_SPAWN(_pt_, &name##_pointer, name(&name##_pointer));\
		PT_END(_pt_);\
	}\
	struct pt name##_rec_pointer

#define getBlockArg(name, i)\
	name##_args[i]

#define THREAD(name)\
	PT_THREAD(name(pt * _pt_))

#define threadBegin()\
	PT_BEGIN(_pt_)

#define threadEnd()\
	PT_END(_pt_)

#define eventBegin()\
	PT_BEGIN(_pt_)

#define eventEnd()\
	PT_YIELD_UNTIL(_pt_, false);\
	PT_END(_pt_)

#define blockBegin()\
	PT_BEGIN(_pt_);\
	PT_YIELD(_pt_)

#define blockEnd()\
	PT_EXIT(_pt_);\
	PT_END(_pt_)

#define wait(seconds)\
	PT_SLEEP(_pt_, (float)seconds * 1000)

#define yield()\
	PT_YIELD(_pt_)

#define yieldUntil(condition)\
	PT_YIELD_UNTIL(_pt_, condition)

#define waitUntil(condition)\
	PT_WAIT_UNTIL(_pt_, condition)

#define waitWhile(condition)\
	PT_WAIT_WHILE(_pt_, condition)

#define initEvent(name)\
	PT_INIT(&name##_pointer)

#define scheduleEvent(name)\
	PT_SCHEDULE(name(&name##_pointer))

#define spawnBlock(...) UTIL_GET_MACRO(_0,##__VA_ARGS__,_sb17,_sb16,_sb15,\
	_sb14,_sb13,_sb12,_sb11,_sb10,_sb9,_sb8,_sb7,_sb6,_sb5,_sb4,_sb3,_sb2,\
	_sb1)(__VA_ARGS__)
#define _sba(n,c,a) n##_args[c] = a
#define _sbs(n) PT_SPAWN(_pt_, &n##_rec_pointer, n##_rec(&n##_rec_pointer))
#define  _sb1(n)_sbs(n)
#define  _sb2(n,a0)                                                    _sba(n,0, a0 );_sb1 (n)
#define  _sb3(n,a0,a1)                                                 _sba(n,1, a1 );_sb2 (n,a0)
#define  _sb4(n,a0,a1,a2)                                              _sba(n,2, a2 );_sb3 (n,a0,a1)
#define  _sb5(n,a0,a1,a2,a3)                                           _sba(n,3, a3 );_sb4 (n,a0,a1,a2)
#define  _sb6(n,a0,a1,a2,a3,a4)                                        _sba(n,4, a4 );_sb5 (n,a0,a1,a2,a3)
#define  _sb7(n,a0,a1,a2,a3,a4,a5)                                     _sba(n,5, a5 );_sb6 (n,a0,a1,a2,a3,a4)
#define  _sb8(n,a0,a1,a2,a3,a4,a5,a6)                                  _sba(n,6, a6 );_sb7 (n,a0,a1,a2,a3,a4,a5)
#define  _sb9(n,a0,a1,a2,a3,a4,a5,a6,a7 )                              _sba(n,7, a7 );_sb8 (n,a0,a1,a2,a3,a4,a5,a6)
#define _sb10(n,a0,a1,a2,a3,a4,a5,a6,a7,a8)                            _sba(n,8, a8 );_sb9 (n,a0,a1,a2,a3,a4,a5,a6,a7)
#define _sb11(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)                         _sba(n,9, a9 );_sb10(n,a0,a1,a2,a3,a4,a5,a6,a7,a8)
#define _sb12(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)                     _sba(n,10,a10);_sb11(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define _sb13(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)                 _sba(n,11,a11);_sb12(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
#define _sb14(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)             _sba(n,12,a12);_sb13(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)
#define _sb15(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)         _sba(n,13,a13);_sb14(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
#define _sb16(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)     _sba(n,14,a14);_sb15(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)
#define _sb17(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) _sba(n,15,a15);_sb16(n,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)



/**
* OLD SIMPLIFIED INTERFACE, keeping here for backwards compatibility
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
