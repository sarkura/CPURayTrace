#include "ThreadPool.h"
#include "Profile.h"


//ThreadPool Thread_Pool = { {true, 395, 252}, 0 };
ThreadPool Thread_Pool = { {false, 0, 0}, 0 };

void ThreadPool::WorkerThread(ThreadPool* ThreadPoolMaster) 
{
	while (ThreadPoolMaster->bThreadPoolStart)
	{
		if (ThreadPoolMaster->Tasks.empty()) 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}
		Task* task = ThreadPoolMaster->GetTask();
		if (task != nullptr) 
		{
			task->Run();
			delete task;
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

void ThreadPool::ParallelFor(size_t InWidth, size_t InHeight, const std::function<void(size_t, size_t)>& Lambda, bool bComplex)
{
	
	PROFILE("ParallelFor");
	Guard GuardLock(TaskLock);
	if (DefaultParallelForDebugInfo.bOpenThreadDebug)
	{
		Task* NewTask = new ParallelForTask(DefaultParallelForDebugInfo.X, DefaultParallelForDebugInfo.Y, DefaultParallelForDebugInfo.ChunkWidth, DefaultParallelForDebugInfo.ChunkWidth, Lambda);
		if (NewTask)
		{
			NewTask->Run();
		}
	}
	else
	{
		float ChunkWidthFloat = static_cast<float>(InWidth) / std::sqrt(static_cast<float>(Threads.size()));
		float ChunkHeightFloat = static_cast<float>(InHeight) / std::sqrt(static_cast<float>(Threads.size()));
		if (bComplex)
		{
			ChunkWidthFloat /= std::sqrt(16.f);
			ChunkHeightFloat /= std::sqrt(16.f);
		}

		size_t ChunkWidth = (size_t)std::ceil(ChunkWidthFloat);
		size_t ChunkHeight = (size_t)std::ceil(ChunkHeightFloat);

		for (size_t i = 0; i < InWidth; i += ChunkWidth)
		{
			for (size_t j = 0; j < InHeight; j += ChunkHeight)
			{
				if (i + ChunkWidth > InWidth)
				{
					ChunkWidth = InWidth - i;
				}
				if (j + ChunkHeight > InHeight)
				{
					ChunkHeight = InHeight - j;
				}

				Task* NewTask = new ParallelForTask(i, j, ChunkWidth, ChunkHeight, Lambda);
				if (NewTask)
				{
					TaskPendingCount += 1;
					Tasks.push(NewTask);	
				}
			}
		}
	}
}

ParallelForTask::ParallelForTask(size_t InWidth, size_t InHeight, size_t InBlockWidth, size_t InBlockHeight, const std::function<void(size_t, size_t)>& FunctionCall)
	: Width(InWidth), Height(InHeight), BlockWidth(InBlockWidth), BlockHeight(InBlockHeight), Lambda(FunctionCall)
{

}

void ParallelForTask::Run()
{
	for (size_t BlockX = 0; BlockX < BlockWidth; BlockX++)
	{
		for (size_t BlockY = 0; BlockY < BlockHeight; BlockY++)
		{
			Lambda(Width + BlockX, Height + BlockY);
		}
	}
}
