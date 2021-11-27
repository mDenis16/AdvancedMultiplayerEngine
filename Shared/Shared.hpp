#pragma once
#include <iostream>
#include <chrono>
#include <queue>
#include <mutex>



#include <unordered_map>

// Our types and definitions
typedef std::chrono::high_resolution_clock Clock;
typedef Clock::time_point ClockTime;
#define ClockDuration std::chrono::duration_cast<std::chrono::milliseconds>

#define SAFE_READ(prop) std::lock_guard <std::mutex> guard(prop##_lock);
#define SAFE_MODIFY(prop) std::lock_guard<std::mutex> guard(prop##_lock);
#define SAFE_PROP(type,prop) \
type prop; \
std::mutex prop##_lock; \

#define VARIABLE_THREAD(poolname) \
std::thread* poolname##thread; \
void poolname(); \
std::mutex poolname##Mutex; \
std::condition_variable poolname##MutexCond; \
std::atomic<bool> poolname##MutexCondFlag; 

#define THREAD_START(poolname,bbind) \
poolname##thread = new std::thread(bbind);

#define THREAD_WAIT(poolname) \
std::unique_lock<std::mutex> lock##poolname(poolname##Mutex); \
poolname##MutexCond.wait_for(lock##poolname, \
std::chrono::seconds(1000), \
[this]() { return poolname##MutexCondFlag.load(); });


//poolname##MutexCond.wait(lock##poolname, [this]{ return poolname##MutexCondFlag.load(); });   

#define THREAD_WAKE(poolname) \
poolname##MutexCondFlag = true; \
poolname##MutexCond.notify_one();

#define THREAD_SLEEP(poolname) \
poolname##MutexCondFlag = false; \
poolname##MutexCond.notify_one();

#define COMMA ,

#include <glm/glm.hpp>
#include <enet\enet.h>
#include "Definitions.h"
#include "ClientCmd\LagRecord.hpp"
#include "Network\NetworkPacket.hpp"
#include "Entities\Entities.hpp"

