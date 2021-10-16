#pragma once
#include "Entities\Entities.hpp"
#include "Definitions.h"
#include "Network\NetworkPacket.hpp"

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
std::mutex poolname##Mutex; \
std::condition_variable poolname##MutexCond; \
bool poolname##MutexCondFlag = false; 


#define THREAD_WAIT(poolname) \
std::unique_lock<std::mutex> lock##poolname(poolname##Mutex); \
poolname##MutexCond.wait_for(lock##poolname, std::chrono::seconds(1000), [this]() { return poolname##MutexCondFlag; }); \

#define THREAD_WAKE(poolname) \
poolname##MutexCondFlag = true; \
poolname##MutexCond.notify_one();

#define THREAD_SLEEP(poolname) \
poolname##MutexCondFlag = false; 

#define COMMA ,