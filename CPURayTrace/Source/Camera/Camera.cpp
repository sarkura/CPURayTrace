#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(Film& InFilm, const glm::vec3& InPos, const glm::vec3 ViewPoint, float FovY)
	:CameraFilm(InFilm), Pos(InPos)
{
	float FilmRadians = glm::radians(FovY);
	float FlimAspect = static_cast<float>(InFilm.GetWidth()) / static_cast<float>(InFilm.GetHeight());
	float ZNear = 1.0f;
	float ZFar = 2.0f;
	glm::mat4 CameraToClipInVerse = glm::perspective(FilmRadians, FlimAspect, ZNear, ZFar);
	CameraFromClip = glm::inverse(CameraToClipInVerse);

	glm::vec3 Eye = Pos;
	glm::vec3 Center = ViewPoint;
	glm::vec3 UpVector(0.f, 1.f, 0.f);
	glm::mat4 WorldToCameraInVerse = glm::lookAt(Eye, Center, UpVector);
	WorldFromCamera = glm::inverse(WorldToCameraInVerse);
}

Camera::~Camera()
{

}

Ray Camera::GenerateRayDirection(const glm::ivec2& PixelCoord, const glm::vec2& Offset /*= {0.5f, 0.5f}*/) const
{
	//First step: Pixed Pos calculate NDC location
	glm::vec2 NDC = (glm::vec2(PixelCoord) + Offset) / glm::vec2(CameraFilm.GetWidth(), CameraFilm.GetHeight());
	//Inverse NDC direction
	NDC.y = 1.f - NDC.y;

	//Map NDC [0,1] -- [-1,1]
	NDC = NDC * 2.f - 1.f;

	//Second Step: Get ClipSpace Pos 
	//Because near == 1, so (x,y,0,near) -- (x/near,y/near,0,1) 
	glm::vec4 ClipLoc(NDC.x, NDC.y, 0, 1);

	//Third Step: Convert ClipLoc to worldloc space
	glm::vec3 WorldLoc = WorldFromCamera * CameraFromClip * ClipLoc;

	Ray GeneratedRay;
	GeneratedRay.Origin = Pos;
	GeneratedRay.Direction = glm::normalize(WorldLoc - Pos);
	return GeneratedRay;
}

Film& Camera::GetFilm()
{
	return CameraFilm;
}

const Film& Camera::GetFilm() const
{
	return CameraFilm;
}