#pragma once

void TestFunction()
{
	/*int Wight = 1, Height = 72;
	Ray PixelRay = TestCamera.GenerateRayDirection({ Wight ,Height });

	glm::vec3 Reflection = { 1.0f, 1.0f, 1.0f };
	glm::vec3 FinalLinerColor = { 0.0f, 0.0f, 0.0f };

	while (true)
	{
		auto HitResultInfo = TestScene.Intersect(PixelRay);
		if (HitResultInfo.has_value())
		{
			const Material* UsedMaterial = &ErrorMaterial;
			if (HitResultInfo->HitMaterial)
			{
				UsedMaterial = HitResultInfo->HitMaterial;
			}

			FinalLinerColor += Reflection * UsedMaterial->Emissive;
			Reflection *= UsedMaterial->Albedo;

			LogPrint("FinalLinerColor", FinalLinerColor);
			LogPrint("Reflection", Reflection);

			PixelRay.Origin = HitResultInfo->HitPos;

			FrameSpace ReflectRayFrameSpace(HitResultInfo->Normal);

			glm::vec3 LocalRayDirection;
			if (UsedMaterial->bSpecular)
			{
				glm::vec3 LocalViewDirection = ReflectRayFrameSpace.LocalFromWorld(-PixelRay.Direction);
				LocalRayDirection = { -LocalViewDirection.x, LocalViewDirection.y, -LocalViewDirection.z };
			}
			else
			{
				do
				{
					LocalRayDirection = { RandDirection.GetRandom(), RandDirection.GetRandom(), RandDirection.GetRandom() };
				} while (glm::length(LocalRayDirection) > 1.0f);

				if (LocalRayDirection.y < 0.f)
				{
					LocalRayDirection.y = -LocalRayDirection.y;
				}
			}
			PixelRay.Direction = ReflectRayFrameSpace.WorldFromLocal(LocalRayDirection);
		}
		else
		{
			break;
		}
	}

	Film_Test.SetPixel(Wight, Height, FinalLinerColor);*/
}
