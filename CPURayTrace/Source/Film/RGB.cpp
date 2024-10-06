#include "RGB.h"
#include <array>
#include <assert.h>

RGB::RGB(int InR, int InG, int InB):
	R(InR), G(InG), B(InB)
{

}


RGB::RGB(const glm::vec3 LinerColor)
{
	R = glm::clamp<int>((int)(glm::pow(LinerColor.x, 1.f / Gamma) * 255.0f), 0, 255);
	G = glm::clamp<int>((int)(glm::pow(LinerColor.y, 1.f / Gamma) * 255.0f), 0, 255);
	B = glm::clamp<int>((int)(glm::pow(LinerColor.z, 1.f / Gamma) * 255.0f), 0, 255);
}

RGB RGB::GenerateHeatmapRGB(float t)
{
	std::array<RGB, 25> ColorPallet
	{
			RGB { 68, 1, 84 },
			RGB { 71, 17, 100 },
			RGB { 72, 31, 112 },
			RGB { 71, 45, 123 },
			RGB { 68, 58, 131 },

			RGB { 64, 70, 136 },
			RGB { 59, 82, 139 },
			RGB { 54, 93, 141 },
			RGB { 49, 104, 142 },
			RGB { 44, 114, 142 },

			RGB { 40, 124, 142 },
			RGB { 36, 134, 142 },
			RGB { 33, 144, 140 },
			RGB { 31, 154, 138 },
			RGB { 32, 164, 134 },

			RGB { 39, 173, 129 },
			RGB { 53, 183, 121 },
			RGB { 71, 193, 110 },
			RGB { 93, 200, 99 },
			RGB { 117, 208, 84 },

			RGB { 143, 215, 68 },
			RGB { 170, 220, 50 },
			RGB { 199, 224, 32 },
			RGB { 227, 228, 24 },
			RGB { 253, 231, 37 },
	};
	if (t < 0.f || t >= 1.0f)
	{
		return RGB(255, 0, 0);
	}
	float LerpStep = t * (ColorPallet.size() - 1);
	int ColorId = (int)glm::floor(LerpStep);
	assert(ColorId >= 0 && ColorId < 24);
	return Lerp(ColorPallet[ColorId], ColorPallet[ ColorId + 1], glm::fract(LerpStep));
}

RGB::operator glm::vec3() const
{
	float RLiner = glm::pow(glm::clamp<float>(static_cast<float>(R) / 255.0f, 0.f, 1.0f), Gamma);
	float GLiner = glm::pow(glm::clamp<float>(static_cast<float>(G) / 255.0f, 0.f, 1.0f), Gamma);
	float BLiner = glm::pow(glm::clamp<float>(static_cast<float>(B) / 255.0f, 0.f, 1.0f), Gamma);
	return glm::vec3(RLiner, GLiner, BLiner);
}

RGB Lerp(const RGB& A, const RGB& B, float t)
{
	int AR = A.GetRedChannel();
	int AG = A.GetGreenChannel();
	int AB = A.GetBlueChannel();

	int BR = B.GetRedChannel();
	int BG = B.GetGreenChannel();
	int BB = B.GetBlueChannel();

	int LerpedR = glm::clamp<int>((int)(AR + (BR - AR) * t), 0, 255);
	int LerpedG = glm::clamp<int>((int)(AG + (BG - AG) * t), 0, 255);
	int LerpedB = glm::clamp<int>((int)(AB + (BB - AB) * t), 0, 255);

	RGB LerpedRGB(LerpedR, LerpedG, LerpedB);
	return LerpedRGB;
}
