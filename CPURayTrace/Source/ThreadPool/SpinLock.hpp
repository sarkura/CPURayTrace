#pragma
#include <atomic>
#include <thread>

class SpinLock
{
public:

	SpinLock() = default;

	void acquire()
	{
		while (flag.test_and_set(std::memory_order_acquire))
		{
			std::this_thread::yield();
		}
	}

	void release()
	{
		flag.clear(std::memory_order_release);
	}

private:
	std::atomic_flag flag{};
};

class Guard
{
public:
	Guard(SpinLock& InSpinLock) : GuardSpinLock(InSpinLock)
	{
		GuardSpinLock.acquire();
	}

	~Guard()
	{
		GuardSpinLock.release();
	}
private:
	SpinLock& GuardSpinLock;
};