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
#include "ConductorMaterial.h"
#include "DielectricMaterial.h"
#include "GroundMaterial.h"


int main()
{

	Film Film_Test(192 * 4, 108 * 4);

    Camera TestCamera(Film_Test, { -10.0f, 1.5f, 0.0f }, { 0.f, 0.f, 0.f }, 45.f);

	Sphere TestSphere({ 0.f, 0.f, 0.f },1.0f );
    Model TestModel("Asset/Models/dragon_871k.obj");
    Plane TestPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
    
    Scene TestScene;

	for (int i = -3; i <= 3; i++) 
	{
		TestScene.AddShape(
			&TestSphere,
			new DielectricMaterial{ 1.f + 0.2f * (i + 3), { 1, 1, 1 } },
			{ 0, 0.5, i * 2 },
			{ 0.8, 0.8, 0.8 }
		);
	}
	for (int i = -3; i <= 3; i++) 
	{
		glm::vec3 C = RGB::GenerateHeatmapRGB((i + 3.f) / 6.f);
		TestScene.AddShape(
			&TestSphere,
			new ConductorMaterial{
				glm::vec3(2.f - C * 2.f),
				glm::vec3(2.f + C * 3.f),
			},
			{ 0, 2.5, i * 2 },
			{ 0.8, 0.8, 0.8 }
			);
	}

	TestScene.AddShape(
		&TestModel,
		new DielectricMaterial { 1.8f, RGB(128, 191, 131) },
		{ -5, 0.4, 1.5 },
		{ 2, 2, 2 }
	);
	TestScene.AddShape(
		&TestModel,
		new ConductorMaterial { { 0.1, 1.2, 1.8 }, { 5, 2.5, 2 } },
		{ -5, 0.4, -1.5 },
		{ 2, 2, 2 }
	);

	TestScene.AddShape(&TestPlane, new GroundMaterial{ RGB(120, 204, 157) }, { 0, -0.5, 0 });
	auto* light_material = new DiffuseMaterial{ { 1, 1, 1 } };
	light_material->SetEmissive({ 0.95, 0.95, 1 });
	TestScene.AddShape(&TestPlane, light_material, { 0, 10, 0 });
	TestScene.BuildTree();
    /*NormalRenderer* TestNormalRenderer = new NormalRenderer(TestCamera, TestScene);
	TestNormalRenderer->Render(1, "Output/Normal.ppm");
    delete TestNormalRenderer;

    BoundsTestCountRenderer* TestBoundsTestCountRenderer = new BoundsTestCountRenderer(TestCamera, TestScene);
    TestBoundsTestCountRenderer->Render(1, "Output/BoundsTestCount.ppm");
    delete TestBoundsTestCountRenderer;

    TriangleTestCountRenderer* TestTriangleTestCountRenderer = new TriangleTestCountRenderer(TestCamera, TestScene);
    TestTriangleTestCountRenderer->Render(1, "Output/TriangleTestCount.ppm");
    delete TestTriangleTestCountRenderer;*/

	PathTraceRenderer* TestPathTraceRenderer = new PathTraceRenderer(TestCamera, TestScene);
	TestPathTraceRenderer->Render(128, "Output/PathTrace.ppm");
	delete TestPathTraceRenderer;

    return 0;
}
