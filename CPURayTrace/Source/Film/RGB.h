#pragma once

#include "glm/glm.hpp"

class RGB
{
public:
	RGB(int InR, int InG, int InB);
	RGB(const glm::vec3 LinerColor);

	operator glm::vec3() const;

	inline int GetRedChannel() const { return R; };
	inline int GetGreenChannel() const { return G; };
	inline int GetBlueChannel() const { return B; };
public:
	static RGB GenerateHeatmapRGB(float t);
private:
	int R, G, B;
	const float Gamma = 2.2f;
};

inline RGB Lerp(const RGB& A, const RGB& B, float t);