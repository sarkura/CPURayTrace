#include "RGB.h"

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

RGB::operator glm::vec3() const
{
	float RLiner = glm::pow(glm::clamp<float>(static_cast<float>(R) / 255.0f, 0.f, 1.0f), Gamma);
	float GLiner = glm::pow(glm::clamp<float>(static_cast<float>(G) / 255.0f, 0.f, 1.0f), Gamma);
	float BLiner = glm::pow(glm::clamp<float>(static_cast<float>(B) / 255.0f, 0.f, 1.0f), Gamma);
	return glm::vec3(RLiner, GLiner, BLiner);
}