#pragma once

#include "Model.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "rapidobj.hpp"
#include "Profile.h"

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
	PROFILE(Model_LoadFile);
	auto LoadResult = rapidobj::ParseFile(FilePath, rapidobj::MaterialLibrary::Ignore());

	for (auto& LoadShape : LoadResult.shapes)
	{
		size_t Index_Offset = 0;
		for (size_t num_face_vectex : LoadShape.mesh.num_face_vertices) {
			if (num_face_vectex == 3) {
				auto Index = LoadShape.mesh.indices[Index_Offset];
				glm::vec3 Pos0{
					LoadResult.attributes.positions[Index.position_index * 3 + 0],
					LoadResult.attributes.positions[Index.position_index * 3 + 1],
					LoadResult.attributes.positions[Index.position_index * 3 + 2]
				};
				Index = LoadShape.mesh.indices[Index_Offset + 1];
				glm::vec3 Pos1{
					LoadResult.attributes.positions[Index.position_index * 3 + 0],
					LoadResult.attributes.positions[Index.position_index * 3 + 1],
					LoadResult.attributes.positions[Index.position_index * 3 + 2]
				};
				Index = LoadShape.mesh.indices[Index_Offset + 2];
				glm::vec3 Pos2{
					LoadResult.attributes.positions[Index.position_index * 3 + 0],
					LoadResult.attributes.positions[Index.position_index * 3 + 1],
					LoadResult.attributes.positions[Index.position_index * 3 + 2]
				};

				if (Index.normal_index >= 0) {
					Index = LoadShape.mesh.indices[Index_Offset];
					glm::vec3 Normal0{
						LoadResult.attributes.normals[Index.normal_index * 3 + 0],
						LoadResult.attributes.normals[Index.normal_index * 3 + 1],
						LoadResult.attributes.normals[Index.normal_index * 3 + 2]
					};
					Index = LoadShape.mesh.indices[Index_Offset + 1];
					glm::vec3 Normal1{
						LoadResult.attributes.normals[Index.normal_index * 3 + 0],
						LoadResult.attributes.normals[Index.normal_index * 3 + 1],
						LoadResult.attributes.normals[Index.normal_index * 3 + 2]
					};
					Index = LoadShape.mesh.indices[Index_Offset + 2];
					glm::vec3 Normal2{
						LoadResult.attributes.normals[Index.normal_index * 3 + 0],
						LoadResult.attributes.normals[Index.normal_index * 3 + 1],
						LoadResult.attributes.normals[Index.normal_index * 3 + 2]
					};
					Triangles.push_back(Triangle{
						Pos0, Pos1, Pos2, Normal0, Normal1, Normal2
						});
				}
				else {
					Triangles.push_back(Triangle{
						Pos0, Pos1, Pos2
						});
				}
			}
			Index_Offset += num_face_vectex;
		}
	}
	return true;
}
