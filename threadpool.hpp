#ifndef __THREAD_POOL_HPP__
#define __THREAD_POOL_HPP__ 

 
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <functional>
#include <condition_variable>

 
namespace tp {

	class ThreadPool {

		std::vector<std::unique_ptr<std::thread>> threads;
		std::list<std::function<void(void)>> queue;

		std::atomic_int         remainJobs;
		std::atomic_bool        bIsDone;
		std::atomic_bool        bIsFinished;
		std::condition_variable jobCondition;
		std::condition_variable waitCondition;
		std::mutex              waitMutex;
		std::mutex              queueMutex;

		void Task() {
			while (!bIsDone) {
				nextJob()();
				--remainJobs;
				waitCondition.notify_one();
			}
		}

		std::function<void(void)> nextJob() {
			std::function<void(void)> job;
			std::unique_lock<std::mutex> jobLock(queueMutex);

			jobCondition.wait(jobLock, [this]() ->bool { return queue.size() || bIsDone; });

			if (!bIsDone) {
				job = queue.front();
				queue.pop_front();
			} else { 
				job = [] {};
				++remainJobs;
			}

			return job;
		}

 

	public:

		ThreadPool(unsigned int ThreadCount = 1)
			: remainJobs(0), bIsDone(false), bIsFinished(false) {

			if (ThreadCount <= 0)
				ThreadCount = 1;
			
			threads.resize(ThreadCount);
			for (unsigned i = 0; i < ThreadCount; ++i) {
				this->threads[i].reset(new std::thread([this] { this->Task(); }));
			}
		}

		~ThreadPool() {
			JoinAll();
		}

		void AddJob(std::function<void(void)> job) {
			std::lock_guard<std::mutex> guard(queueMutex);
			queue.emplace_back(job);
			++remainJobs;
			jobCondition.notify_one();
		}

		void JoinAll(bool WaitForAll = true) {
			if (!bIsFinished) {
				if (WaitForAll) {
					WaitAll();
				}

				bIsDone = true;
				jobCondition.notify_all();

				for (auto &th : threads)
					if (th->joinable())
						th->join();
 
				queue.clear();
				threads.clear();
				bIsFinished = true;
			}
		}

		void WaitAll() {
			if (remainJobs > 0) {
				std::unique_lock<std::mutex> waitLock(waitMutex);
				waitCondition.wait(waitLock, [this] { return this->remainJobs == 0; });
				waitLock.unlock();
			}
		}
	};
}
 

#endif