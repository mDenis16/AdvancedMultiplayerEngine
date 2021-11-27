#pragma once


namespace NativeThreadSafe {

	class QueueJob;

	inline std::atomic_bool runningThreadSafe = true;
	inline std::mutex QueueJobMutex;
	inline std::queue<QueueJob*> QueueJobs;
	class QueueJob {
	public:


		std::mutex ConditionalMutex;
		std::condition_variable Conditional;
		std::atomic<bool> Executed;
		std::vector<std::uint64_t> Callbacks;

		std::function<void()> FunctionExecute;
		void WaitforCallback() {

			std::unique_lock<std::mutex> lock(ConditionalMutex);
			Conditional.wait(lock, [this] { return Executed.load(); });

		}
		~QueueJob() {
			//	std::cout << "destructed shit" << std::endl;
		}
		QueueJob() {
			//std::cout << "constructed shit " << std::endl;
		}

		void Execute(std::function<void()> const& func) {
			if (func)
			{
				FunctionExecute = std::move(func);

				std::unique_lock<std::mutex> lock(NativeThreadSafe::QueueJobMutex);
				NativeThreadSafe::QueueJobs.push(this);
				//	std::cout << "Pushed queue " << std::endl;
			}
		}

	};


	void ExecuteNativeQueue();

}

#define NATIVE_FUNCTION(func, callbacksSize) \
{ \
NativeThreadSafe::QueueJob* Job = new NativeThreadSafe::QueueJob(); \
Job->Execute([Job] \
	{ \
	Job->Callbacks.resize(callbacksSize);\
	func\
	});\
Job->WaitforCallback();


#define NATIVE_END() \
delete Job; \
}