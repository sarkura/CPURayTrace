#pragma once

#include "glm/glm.hpp"
#include <iostream>

void LogPrint(std::string Name, glm::vec3 V)
{
	std::cout << Name << " x:" << V.x << " y:" << V.y << " z:" << V.z << std::endl;
}
