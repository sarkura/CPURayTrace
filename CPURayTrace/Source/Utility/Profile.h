#pragma once

#include <chrono>
#include <iostream>

#define PROFILE(Name) Profile __Profile(Name);

class Profile
{
public:
	Profile(const std::string& InName);
	~Profile();
private:
	std::chrono::high_resolution_clock::time_point StartTime;
	std::string Name;
};
