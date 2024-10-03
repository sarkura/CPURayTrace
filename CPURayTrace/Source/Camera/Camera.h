#pragma once

#include "Film.h"
#include "Ray.h"

class Camera
{
public:
	Camera(Film& InFilm, const glm::vec3& InPos, const glm::vec3 InViewPoint, float InFovY);

	~Camera();

	Ray GenerateRayDirection(const glm::ivec2& PixelCoord, const glm::vec2& Offset = { 0.5f, 0.5f }) const;

	Film& GetFilm();

	const Film& GetFilm() const;
private:

	Film& CameraFilm;

	//Camera Pos in World Space
	glm::vec3 Pos;

	glm::mat4 CameraFromClip;
	glm::mat4 WorldFromCamera;
};


