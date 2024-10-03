#pragma once

#include "Model.h"

#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const std::vector<Triangle> InTriangles) :
	Triangles(InTriangles)
{

}

Model::Model(const std::filesystem::path& FilePath)
{
	if (!LoadFile(FilePath))
	{
		std::cout << "Load File " << FilePath << " Failed!" << std::endl;
	}
}

std::optional<HitInfo> Model::Intersect(const Ray& InRay, float T_Min /*= 1e-5*/, float T_Max /*= std::numeric_limits<float>::infinity()*/) const
{
	std::optional<HitInfo> ResultInfo;

	float MaxStep = T_Max;
	for (const Triangle& CurTriangle : Triangles)
	{
		std::optional<HitInfo> CurHitInfo = CurTriangle.Intersect(InRay, T_Min, MaxStep);
		if (CurHitInfo.has_value())
		{
			MaxStep = CurHitInfo->TStep;
			ResultInfo = CurHitInfo;
		}
	}

	return ResultInfo;
}

bool Model::LoadFile(const std::filesystem::path& FilePath)
{
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;

	std::ifstream File(FilePath);
	if (!File.good())
	{
		//std::cout << "Open File " << FilePath << " Failed!" << std::endl;
		return false;
	}

	std::string Line;
	char Trash;
	while (!File.eof())
	{
		std::getline(File, Line);
		std::istringstream Iss(Line);
		if (Line.compare(0, 2, "v ") == 0)
		{
			glm::vec3 Position;
			Iss >> Trash >> Position.x >> Position.y >> Position.z;
			Positions.push_back(Position);
		}
		else if (Line.compare(0, 2, "vn") == 0)
		{
			glm::vec3 Normal;
			Iss >> Trash >> Trash >> Normal.x >> Normal.y >> Normal.z;
			Normals.push_back(Normal);
		}
		else if (Line.compare(0, 2, "f ") == 0)
		{
			glm::ivec3 IdV, IdVN;
			Iss >> Trash;
			Iss >> IdV.x >> Trash >> Trash >> IdVN.x;
			Iss >> IdV.y >> Trash >> Trash >> IdVN.y;
			Iss >> IdV.z >> Trash >> Trash >> IdVN.z;
			Triangles.push_back(Triangle(
				Positions[IdV.x - 1], Positions[IdV.y - 1], Positions[IdV.z - 1],
				Normals[IdVN.x - 1], Normals[IdVN.y - 1], Normals[IdVN.z - 1]
			));
		}
	}
	return true;
}
