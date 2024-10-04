#include "ThreadPool.h"

#include "Profile.h"

//ThreadPool Thread_Pool = { {true, 10, 20}, 0 };
ThreadPool Thread_Pool;

void ThreadPool::WorkerThread(ThreadPool* ThreadPoolMaster) 
{
	while (ThreadPoolMaster->bThreadPoolStart)
	{
		Task* task = ThreadPoolMaster->GetTask();
		if (task != nullptr) 
		{
			task->Run();
			ThreadPoolMaster->TaskPendingCount -= 1;
		}
		else 
		{
			std::this_thread::yield();
		}
	}
}

ThreadPool::ThreadPool(const ParallelForDebugInfo& InDefaultParallelForDebugInfo, size_t ThreadCount)
{
	DefaultParallelForDebugInfo = InDefaultParallelForDebugInfo;
	if (DefaultParallelForDebugInfo.bOpenThreadDebug)
	{
		TaskPendingCount = 0;
	}
	else
	{
		bThreadPoolStart = true;
		TaskPendingCount = 0;
		if (ThreadCount == 0)
		{
			ThreadCount = std::thread::hardware_concurrency();
		}
		for (size_t i = 0; i < ThreadCount; i++)
		{
			Threads.push_back(std::thread(ThreadPool::WorkerThread, this));
		}
	}
}

ThreadPool::~ThreadPool() 
{
	Wait();
	bThreadPoolStart = false;
	for (auto& Thread : Threads) 
	{
		Thread.join();
	}
	Threads.clear();
}

void ThreadPool::AddTask(Task* InTask) 
{
	Guard GuardLock(TaskLock);
	Tasks.push(InTask);
	TaskPendingCount += 1;
}

Task* ThreadPool::GetTask() 
{
	Guard GuardLock(TaskLock);
	if (Tasks.empty()) 
	{
		return nullptr;
	}
	Task* OutTask = Tasks.front();
	Tasks.pop();
	return OutTask;
}

void ThreadPool::Wait() const
{
	while (TaskPendingCount > 0)
	{
		std::this_thread::yield();
	}
}

void ThreadPool::ParallelFor(size_t InWidth, size_t InHeight, const std::function<void(size_t, size_t)>& Lambda)
{
	//Guard GuardLock(TaskLock);
	PROFILE(ThreadPool_ParallelFor);
	if (DefaultParallelForDebugInfo.bOpenThreadDebug)
	{
		Task* NewTask = new ParallelForTask(DefaultParallelForDebugInfo.X, DefaultParallelForDebugInfo.Y, Lambda);
		if (NewTask)
		{
			NewTask->Run();
		}
	}
	else
	{
		//float ChunkWidth = static_cast<float>(InWidth) / std::sqrt(16.f) / std::sqrt(Threads.size());
		//float ChunkHeight = static_cast<float>(InHeight) / std::sqrt(16.f) / std::sqrt(Threads.size());

		for (size_t i = 0; i < InWidth; i++)
		{
			for (size_t j = 0; j < InHeight; j++)
			{
				Task* NewTask = new ParallelForTask(i, j, Lambda);
				if (NewTask)
				{
					AddTask(NewTask);
				}
			}
		}
	}
}

ParallelForTask::ParallelForTask(size_t InWidth, size_t InHeight, const std::function<void(size_t, size_t)>& FunctionCall) 
	: Width(InWidth), Height(InHeight), Lambda(FunctionCall)
{

}

void ParallelForTask::Run()
{
	Lambda(Width, Height);
}
