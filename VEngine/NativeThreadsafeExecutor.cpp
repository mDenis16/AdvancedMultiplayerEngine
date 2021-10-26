#include "pch.h"

#include "NativeThreadsafeExecutor.hpp"
#include <iostream>

void NativeThreadSafe::ExecuteNativeQueue()
{
	try {
		if (QueueJobMutex.try_lock()) {
			while (QueueJobs.size() > 0) {

				std::cout << "Executed task" << std::endl;

				auto job = QueueJobs.front();
				QueueJobs.pop();

				job->FunctionExecute();
				std::unique_lock<std::mutex> lock(job->ConditionalMutex);
				job->Executed = true;
				job->Conditional.notify_all();
			}

			QueueJobMutex.unlock();
		}
	}
	catch (int ex) {
		std::cout << ex << std::endl;
	}
}