#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>

#include "SpinLock.hpp"


struct ParallelForDebugInfo
{
	bool bOpenThreadDebug = false;
	size_t X = 0;
	size_t Y = 0;
};


class Task 
{
public:
	Task() = default;

	virtual void Run() = 0;
};

class ParallelForTask : public Task
{
public:

	explicit ParallelForTask(size_t InWidth, size_t InHeight,const std::function<void(size_t, size_t)>& FunctionCall);

	virtual void Run() override;

private:

	size_t Width;
	size_t Height;

	//call back function
	std::function<void(size_t, size_t)> Lambda;
};

class ThreadPool 
{
public:

	static void WorkerThread(ThreadPool* ThreadPoolMaster);

public:
	ThreadPool(const ParallelForDebugInfo& InDefaultParallelForDebugInfo = {false, 0, 0}, size_t ThreadCount = 0);
	~ThreadPool();

	ThreadPool(ThreadPool&) = delete;

	void AddTask(Task* InTask);
	Task* GetTask();

	void Wait() const;

	void ParallelFor(size_t InWidth, size_t InHeight, const std::function<void(size_t, size_t)>& Lambda);

private:
	std::atomic_bool bThreadPoolStart;
	std::vector<std::thread> Threads;
	std::queue<Task*> Tasks;
	SpinLock TaskLock;
	std::atomic_int TaskPendingCount;

	ParallelForDebugInfo DefaultParallelForDebugInfo;
};

extern ThreadPool Thread_Pool;