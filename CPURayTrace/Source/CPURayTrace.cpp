// CPURayTrace.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "glm/glm.hpp"

#include "Film.h"
#include "RGB.h"
#include "Camera.h"

#include "Sphere.h"
#include "Plane.h"
#include "Model.h"
#include "Scene.h"

#include "DiffuseMaterial.h"
#include "SpecularMaterial.h"
#include "NormalRenderer.h"
#include "DebugRenderer.h"
#include "PathTraceRenderer.h"


#include "Log.hpp"


int main()
{

	Film Film_Test(192 * 4, 108 * 4);

    Camera TestCamera(Film_Test, { -12.0f, 5.0f, -12.0f }, { 0.f, 0.f, 0.f }, 45.f);

	Sphere TestSphere({ 0.f, 0.f, 0.f },1.0f );
    Model TestModel("Asset/Models/dragon_871k.obj");
    Plane TestPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
    
    Scene TestScene;

	RandomDistribution<float> Range(1234, 0.f, 1.0f);
	for (int i = 0; i < 10000; i++) 
	{
		glm::vec3 random_pos{
			Range.GetRandom() * 100 - 50,
			Range.GetRandom() * 2,
			Range.GetRandom() * 100 - 50,
		};
		float u = Range.GetRandom();
		if (u < 0.9) 
		{
			if (Range.GetRandom() > 0.5)
			{
				SpecularMaterial* NewSpecularMaterial = new SpecularMaterial({ RGB(202, 159, 117) });
				TestScene.AddShape(
					&TestModel,
					NewSpecularMaterial,
					random_pos,
					{ 1, 1, 1 },
					{ Range.GetRandom() * 360, Range.GetRandom() * 360, Range.GetRandom() * 360 }
				);
			}
			else
			{
				DiffuseMaterial* NewDiffuseMaterial = new DiffuseMaterial(RGB(202, 159, 117));
				TestScene.AddShape(
					&TestModel,
					NewDiffuseMaterial,
					random_pos,
					{ 1, 1, 1 },
					{ Range.GetRandom() * 360, Range.GetRandom() * 360, Range.GetRandom() * 360 }
				);
			}
			
		}
		else if (u < 0.95) {
			SpecularMaterial* NewSpecularMaterial = new SpecularMaterial( { Range.GetRandom(), Range.GetRandom(), Range.GetRandom() });
			TestScene.AddShape(
				&TestSphere,
				NewSpecularMaterial,
				random_pos,
				{ 0.4, 0.4, 0.4 }
			);
		}
		else {
			random_pos.y += 6;
			DiffuseMaterial* NewDiffuseMaterial = new DiffuseMaterial({ 1, 1, 1 });
			NewDiffuseMaterial->SetEmissive({ Range.GetRandom() * 4, Range.GetRandom() * 4, Range.GetRandom() * 4 });
			TestScene.AddShape(
				&TestSphere,
				NewDiffuseMaterial,
				random_pos
			);
		}
	}
	DiffuseMaterial* NewPlanDiffuseMaterial = new DiffuseMaterial(RGB(120, 204, 157));
	TestScene.AddShape(&TestPlane, NewPlanDiffuseMaterial, { 0, -0.5, 0 });
	TestScene.BuildTree();
    NormalRenderer* TestNormalRenderer = new NormalRenderer(TestCamera, TestScene);
	TestNormalRenderer->Render(1, "Output/Normal.ppm");
    delete TestNormalRenderer;

    BoundsTestCountRenderer* TestBoundsTestCountRenderer = new BoundsTestCountRenderer(TestCamera, TestScene);
    TestBoundsTestCountRenderer->Render(1, "Output/BoundsTestCount.ppm");
    delete TestBoundsTestCountRenderer;

    TriangleTestCountRenderer* TestTriangleTestCountRenderer = new TriangleTestCountRenderer(TestCamera, TestScene);
    TestTriangleTestCountRenderer->Render(1, "Output/TriangleTestCount.ppm");
    delete TestTriangleTestCountRenderer;

	PathTraceRenderer* TestPathTraceRenderer = new PathTraceRenderer(TestCamera, TestScene);
	TestPathTraceRenderer->Render(128, "Output/PathTrace.ppm");
	delete TestPathTraceRenderer;

    return 0;
}
