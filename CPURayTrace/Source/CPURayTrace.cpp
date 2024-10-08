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

#include "Material.h"
#include "NormalRenderer.h"
#include "SimpleRayTraceRenderer.h"
#include "DebugRenderer.h"

#include "Log.hpp"
#include "PathTraceRenderer.h"

int main()
{

	Film Film_Test(192 * 4, 108 * 4);

    Camera TestCamera(Film_Test, { -12.0f, 5.0f, -12.0f }, { 0.f, 0.f, 0.f }, 45.f);

	Sphere TestSphere({ 0.f, 0.f, 0.f },1.0f );

    Model TestModel("Asset/Models/dragon_871k.obj");

    Plane TestPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });


    /*Material TestSphereMaterialA;
    TestSphereMaterialA.Albedo = {1.0f, 1.0f, 1.0f};
    TestSphereMaterialA.bSpecular = false;
    TestSphereMaterialA.Emissive = RGB(255, 128, 128);

	Material TestSphereMaterialB;
	TestSphereMaterialB.Albedo = { 1.0f, 1.0f, 1.0f };
	TestSphereMaterialB.bSpecular = false;
	TestSphereMaterialB.Emissive = RGB(128, 128, 255);

	Material TestSphereMaterialC;
	TestSphereMaterialC.Albedo = { 1.0f, 1.0f, 1.0f };
	TestSphereMaterialC.bSpecular = true;

    Material TestModelMaterial;
    TestModelMaterial.Albedo = RGB(202, 159, 117);

    
    Material TestPlaneMaterial;
    TestPlaneMaterial.Albedo = RGB(120, 204, 157);

    Material ErrorMaterial;*/
    
    Scene TestScene;

    /*TestScene.AddShape(&TestModel, TestModelMaterial, { 0.f, 0.f, 0.f }, { 3.f, 3.f, 3.f });
    TestScene.AddShape(&TestSphere, TestSphereMaterialA, { 0.f, 0.f, 2.5f });
    TestScene.AddShape(&TestSphere, TestSphereMaterialB, { 0.f, 0.f, -2.5f });
    TestScene.AddShape(&TestSphere, TestSphereMaterialC, { 3.f, 0.5f, -2.f });
    TestScene.AddShape(&TestPlane, TestPlaneMaterial, { 0, -0.5, 0 });*/

	RandomDistribution<float> Range(1234, 0.f, 1.0f);
	for (int i = 0; i < 10000; i++) {
		glm::vec3 random_pos{
			Range.GetRandom() * 100 - 50,
			Range.GetRandom() * 2,
			Range.GetRandom() * 100 - 50,
		};
		float u = Range.GetRandom();
		if (u < 0.9) {
			TestScene.AddShape(
				&TestModel,
				{ RGB(202, 159, 117), Range.GetRandom() > 0.5 },
				random_pos,
				{ 1, 1, 1 },
				{ Range.GetRandom() * 360, Range.GetRandom() * 360, Range.GetRandom() * 360 }
			);
		}
		else if (u < 0.95) {
			TestScene.AddShape(
				&TestSphere,
				{ { Range.GetRandom(), Range.GetRandom(), Range.GetRandom() }, true },
				random_pos,
				{ 0.4, 0.4, 0.4 }
			);
		}
		else {
			random_pos.y += 6;
			TestScene.AddShape(
				&TestSphere,
				{ { 1, 1, 1 }, false, { Range.GetRandom() * 4, Range.GetRandom() * 4, Range.GetRandom() * 4 } },
				random_pos
			);
		}
	}
	TestScene.AddShape(&TestPlane, { RGB(120, 204, 157) }, { 0, -0.5, 0 });
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

	SimpleRayTraceRenderer* TestSimpleRayTraceRenderer = new SimpleRayTraceRenderer(TestCamera, TestScene);
	TestSimpleRayTraceRenderer->Render(128, "Output/SimpleRayTrace.ppm");
	delete TestSimpleRayTraceRenderer;

	PathTraceRenderer* TestPathTraceRenderer = new PathTraceRenderer(TestCamera, TestScene);
	TestPathTraceRenderer->Render(128, "Output/PathTrace.ppm");
	delete TestPathTraceRenderer;

    return 0;
}
