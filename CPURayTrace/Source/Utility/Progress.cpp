#include "Progress.h"
#include <iostream>
#include <assert.h>

Progress::Progress(size_t InTotalPercent, int DefaultStep /*= 1*/):
	TotalPercent(InTotalPercent), UpdateStep(DefaultStep), CurrectPercent(0), LastPercentage(0), Percentage(0)
{
	assert(InTotalPercent > 0);
	std::cout << "Precess :" << Percentage << "%" << std::endl;
}

void Progress::UpdatePercent(size_t InCount)
{
	Guard StepGuard(ProgressLock);

	CurrectPercent += InCount;
	Percentage = (int)(static_cast<float>(CurrectPercent) / static_cast<float>(TotalPercent) * 100.0f);
	if ((Percentage - LastPercentage >= UpdateStep) || (Percentage == 100))
	{
		LastPercentage = Percentage;
		std::cout << "Precess :" << Percentage << "%" << std::endl;
	}
}
