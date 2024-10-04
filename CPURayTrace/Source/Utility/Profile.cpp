#include "Profile.h"

Profile::Profile(const std::string& InName) :
	Name(InName), StartTime(std::chrono::high_resolution_clock::now())
{

}

Profile::~Profile()
{
	auto Duration = std::chrono::high_resolution_clock::now() - StartTime;
	auto MilliSecond = std::chrono::duration_cast<std::chrono::milliseconds>(Duration).count();
	std::cout << "Function: " << Name << "  Duration: " << MilliSecond  << "ms" << std::endl;
}

