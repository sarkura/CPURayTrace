#pragma once

#include "glm/glm.hpp"

class RGB
{
public:
	RGB(int InR, int InG, int InB);
	RGB(const glm::vec3 LinerColor);

	operator glm::vec3() const;

	inline int GetRedChannel() { return R; };
	inline int GetGreenChannel() { return G; };
	inline int GetBlueChannel() { return B; };

private:
	int R, G, B;
	const float Gamma = 2.2f;
};
