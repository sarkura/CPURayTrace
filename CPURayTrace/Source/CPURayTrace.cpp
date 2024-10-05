// CPURayTrace.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "ThirdParty/glm/glm.hpp"

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

#include "Log.hpp"

int main()
{

	Film Film_Test(192 * 4, 108 * 4);

    Camera TestCamera(Film_Test, { -3.6, 0, 0 }, { 0, 0, 0 }, 45);

	Sphere TestSphere{
		{ 0.f, 0.f, 0.f },
		1.0f };

    Model TestModel("Asset/Models/simple_dragon.obj");

    Plane TestPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });


    Material TestSphereMaterialA;
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

    Material ErrorMaterial;
    
    Scene TestScene;

    TestScene.AddShape(&TestModel, &TestModelMaterial, { 0.f, 0.f, 0.f }, { 3.f, 3.f, 3.f });
    //TestScene.AddShape(&TestSphere, &TestSphereMaterialA, { 0.f, 0.f, 2.5f });
    //TestScene.AddShape(&TestSphere, &TestSphereMaterialB, { 0.f, 0.f, -2.5f });
    //TestScene.AddShape(&TestSphere, &TestSphereMaterialC, { 3.f, 0.5f, -2.f });
    //TestScene.AddShape(&TestPlane, &TestPlaneMaterial, { 0, -0.5, 0 });

	NormalRenderer TestNormalRenderer(TestCamera, TestScene);
	TestNormalRenderer.Render(1, "Output/Normal.ppm");

    

    /*SimpleRayTraceRenderer TestSimpleRayTraceRenderer(TestCamera, TestScene);
    TestSimpleRayTraceRenderer.Render(1, "Output/SimpleRayTrace.ppm");*/

    return 0;
}
