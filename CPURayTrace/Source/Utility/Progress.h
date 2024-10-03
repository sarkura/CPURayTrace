#pragma once

#include "ThreadPool.h"

class Progress
{
public:
	Progress(size_t InTotalPercent, int DefaultStep = 1);
	void UpdatePercent(size_t InCount);
private:
	SpinLock ProgressLock;
	int Percentage, LastPercentage, UpdateStep;
	size_t TotalPercent, CurrectPercent;
};
