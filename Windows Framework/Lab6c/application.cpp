#include "application.h"

#include "heightmap.h"
#include "lights.h"
#include "noisegenerator.h"
#include "water.h"

#include <iostream>
#include <time.h>


Application::Application(void):
	gridSize(600.0f),
	myNoise(16, 200.0f, 0.43f, 80.0f, 1.155f),
	skybox("inwardCube.obj")
{
}


Application::~Application(void)
{
}

void Application::Initialize(HINSTANCE hInstance)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create a window, initialise OpenGL context and retrieve		|
|				machine and driver specs									|
|																			|
|	Parameters:	hInstance of the process									|
|																			|
|	Returns:	None.														|
|																			|
\*-------------------------------------------------------------------------*/
{
	srand(51816);
	float rot = 0.0;

	//  Create a new window called OpenGL Framework, 1000 pix long and 500 pix tall
	window.WindowCreate("OpenGL Framework", 1000, 500, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), 0, hInstance);

	//  Create the openGL viewport in the window with a field of view of 45 degrees
	window.InitializeGraphics(45.0f);

	//  Move the cursor to the middle of the window
	window.SetCursorToCentre();

	//  Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//  Initialise glew (needed for all post 1.1 functionality)
	glewInit();


	//  Find the number of texture units we have to play with
	int numTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureUnits);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL 4.2 Initialized: " << 	GLEW_VERSION_4_2 << std::endl;
	std::cout << "Number of Texture Units: " << numTextureUnits << std::endl;

	std::cout << std::endl << "Number of Available Processors: " << GetActiveProcessorCount(ALL_PROCESSOR_GROUPS) << std::endl;

	//  Create all the objects in the scene
	InitialiseScene();

	lastTime = time(NULL);
	nbFrames = 0;
	wireframe = false;
	framerateLogging = false;
	culling = false;
	glCullFace(GL_BACK);
}

void Application::MainLoop()
/*-------------------------------------------------------------------------*\
|	Purpose:	The primary application loop, handles calling of all		|
|				update functions											|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	None.														|
|																			|
\*-------------------------------------------------------------------------*/
{
	//  Slow down incase of really fast machine
	if (input.GetTimeSinceLastFrame() >= 10) {
		//  Lots of debug outputs
		Debug();
		//  Set the window buffers
		window.PrepareForDrawing();
		//  Update the player based on input
		player.InputUpdate();

		//  Rotate the sphere
		testObject.RotateLocalDeltaZ(0.05f);
		testObject.RotateLocalDeltaY(0.025f);
		testObject.RotateLocalDeltaX(0.03f);

		if (input.ReportKeyState(VK_ADD)) {
			testObject.UniformScale(1.01f);
		}

		if (input.ReportKeyState(VK_SUBTRACT)) {
			testObject.UniformScale(0.99f);
		}

		//  Make sure the player doesn't fall through the ground
		player.CheckGroundCollision(myNoise);

		//  Update the input state
		input.Update();

		//  Check for need to build more terrain
		testTerrain.Update();

		//  When we right click lock the mouse to the centre before 
		if(input.ReportRMousePress()) {
			window.SetCursorToCentre();
			window.SetMouseLockedCentre();
		}

		//  Allow the player to speed up or reverse the movement of the sun
		if(input.ReportKeyState(VK_LEFT)) {
			sunParent.RotateDeltaX(0.01f);
		}
		sunParent.RotateDeltaX(-0.001f);

		if (input.ReportKeyState(VK_RIGHT)) {
			sunParent.RotateDeltaX(-0.01f);
		}

		//  Exit the program
		if (input.ReportKeyPress(VK_ESCAPE))
			exit(0);

		//  Render the scene
		renderer.RenderAll();

		window.FlipBuffers();
	}
}

void Application::Debug()
/*-------------------------------------------------------------------------*\
|	Purpose:	Checks for key presses requesting debug information			|
|				and sends the information to console out					|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	None.														|
|																			|
\*-------------------------------------------------------------------------*/
{
	double currentTime = time(NULL);
	nbFrames++;
	if ( currentTime - lastTime >= 1.0 ){
		if (framerateLogging) {
			std::cout << 1000.0/double(nbFrames) << " ms/frame\t" << "(" << nbFrames << "FPS)" << std::endl;
		}
		nbFrames = 0;
		lastTime += 1.0;
	}

	if (input.ReportKeyPress('F')) {
		framerateLogging = !framerateLogging;
	}
	if (input.ReportKeyPress('C')) {
		culling = !culling;
		if (culling) {
			glEnable(GL_CULL_FACE);
			std::cout << "Backface Culling: ON" << std::endl;
		} else {
			glDisable(GL_CULL_FACE);
			std::cout << "Backface Culling: OFF" << std::endl;
		}

	}

	if (input.ReportKeyPress('N')) {
		std::cout << "Last Frame Took: " << input.GetTimeForLastFrame() << "s" << std::endl;
	}

	if (input.ReportKeyPress('L')) {
		std::cout << "Position: (" << player.GetPosition().x << ", "<< player.GetPosition().y << ", " << player.GetPosition().z << std::endl;
		std::cout << "Forward Vector: (" << player.GetLocalZ().x << ", " << player.GetLocalZ().y << ", " << player.GetLocalZ().z << ")\t(Length: " << player.GetLocalZ().Length() << ")" << std::endl;
		std::cout << "Up Vector: (" << player.GetLocalY().x << ", " << player.GetLocalY().y << ", " << player.GetLocalY().z << ")\t(Length: " << player.GetLocalY().Length() << ")" << std::endl;
		std::cout << "Rotation Quaternion: (" << player.GetRotation().s << ", (" << player.GetRotation().x << ", " << player.GetRotation().y << ", " << player.GetRotation().z << "))" << std::endl;
		std::cout << "Rotation Angle: " << player.GetRotation().GetAngle() << "\tRotation Axis: (" << player.GetRotation().GetAxis().x << ", " << player.GetRotation().GetAxis().y << ", " << player.GetRotation().GetAxis().z << ")" << std::endl;
	}

	if (input.ReportKeyPress('I')) {
		wireframe = !wireframe;
		if (wireframe) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			std::cout << "WireFrame Mode: ON" << std::endl;
		} else {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			std::cout << "WireFrame Mode: OFF" << std::endl;
		}
	}
}

void Application::InitialiseScene()
/*-------------------------------------------------------------------------*\
|	Purpose:	Create and set up all the objects and lights in the scene	|
|				push them all to the renderer								|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	None.														|
|																			|
\*-------------------------------------------------------------------------*/
{

	//  Set the player's initial position
	player.SetLocation(50.0f, 50.0f, 50.0f);

	//  Create two lights (one point light fixed to player and one directional light)
	LightSource playerLight(LightSource::POINT);
	playerLight.SetColour(0.7f, 0.7f, 0.7f);
	playerLight.SetAmbient(0.0f);

	LightSource sunSource(LightSource::DIRECTIONAL);
	sunSource.SetColour(0.7f, 0.7f, 0.7f);
	sunSource.SetAmbient(0.1f);

	sunSource.SetParent(sunParent);
	player.SetCameraTargetWindow(&window);
	playerLight.SetParent(player);

	//  Add the camera and lights to the renderer
	renderer.SetActiveCamera(*player.GetCamera());
	renderer.AddLight(sunSource);
	renderer.AddLight(playerLight);

	//  Create the material for the skybox
	Material skyMat;
	skyMat.AddTexture(Texture(Texture::DIFFUSE, "stars2.tga"));
	skyMat.AddShader("skybox.fragmentshader");
	skyMat.AddShader("skybox.vertexshader");

	//  Create the mesh for the skybox (cube with inward facing sides/normals subdivided
	//  4 times then warped into a sphere of radius 1)
	skybox = meshGenerator.InwardCubeSphere(4);

	skybox.AttachMaterial(skyMat);
	renderer.AddSkyBox(skybox);

	PrimitiveFactory prims;

	Material crateMatProc;
	crateMatProc.AddTexture(Texture(Texture::DIFFUSE, "crateDiffuseProc.tga"));
	crateMatProc.AddShader("default.fragmentshader");
	crateMatProc.AddShader("default.vertexshader");

	Material crateMat;
	crateMat.AddTexture(Texture(Texture::DIFFUSE, "crateDiffuse.tga"));
	crateMat.AddShader("default.fragmentshader");
	crateMat.AddShader("default.vertexshader");

	Mesh crateMesh("crate.obj");
	Mesh altCrate(meshGenerator.SubDivide(meshGenerator.UnitCube()));

	altCrate.AttachMaterial(crateMatProc);
	crateMesh.AttachMaterial(crateMat);

	int num = 5;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			for (int k = 0; k < num; k++) {
				StaticObject* curr = new StaticObject();
				curr->AttachMesh(altCrate);
				curr->SetLocation(i*3.0f, 20.0f+j*3.0f, k*3.0f);
				renderer.AddToRenderer(*curr->GetMesh());
			}
		}
	}
	Mesh newAlt(meshGenerator.CubeSphere(4));
	newAlt.AttachMaterial(crateMatProc);
	testObject.AttachMesh(newAlt);
	testObject.SetLocation(100.0f, 40.0f, 100.0f);
	renderer.AddToRenderer(*testObject.GetMesh());

	Mesh boatMesh("boat.obj");
	Material boatMaterial;
	boatMaterial.AddTexture(Texture(Texture::DIFFUSE, "grey.tga"));
	boatMaterial.AddShader("default.vertexshader");
	boatMaterial.AddShader("default.fragmentshader");
	boatMesh.AttachMaterial(boatMaterial);
	boat.AttachMesh(boatMesh);
	boat.SetLocation(105.0f, -12.67f, 44.0f);
	boat.RotateLocalDeltaZ(-0.15f);
	boat.RotateLocalDeltaX(0.05f);
	renderer.AddToRenderer(*boat.GetMesh());

	testTerrain.Initialize(renderer, myNoise);
	renderer.AddTerrainToRenderer(testTerrain);
}

void Application::CrossProductBenchmark()
/*-------------------------------------------------------------------------*\
|	Purpose:	Compare computation time for Cross-Products using SIMD 		|
|				instructions to linear operations.							|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	Sends times to Console out.									|
|																			|
\*-------------------------------------------------------------------------*/
{

	float out = 0;
	double myTimer;
	const int itnum = 100000;

	std::cout << "Timing " << itnum*itnum << " cross products..." << std::endl; 

	SIMD::Floats x(1.2, 3.5, 4.6, 1.0);
	SIMD::Floats y(5.2, 8.1, 4.6, 1.0);

	SIMD::Floats res;

	myTimer = clock();

	for (int i = 0; i < itnum; i++) {
		for (int j = 0; j < itnum; j++) {
			res += SIMD::Cross(x, y);
		}
	}
	myTimer = clock()-myTimer;

	std::cout << res/(itnum*itnum) << std::endl;
	std::cout << "SIMD Op Time: " << myTimer/(CLOCKS_PER_SEC) << "s" << std::endl;

	Vector4 xv(1.2, 3.5, 4.6, 1.0);
	Vector4 yv(5.2, 8.1, 4.6, 1.0);
	Vector4 resv;

	myTimer = clock();

	for (int i = 0; i < itnum; i++) {
		for (int j = 0; j < itnum; j++) {
			resv += xv.Cross(yv);
		}
	}
	myTimer = clock()-myTimer;

	std::cout << resv/(itnum*itnum) << std::endl;
	std::cout << "SISD Op Time: " << myTimer/(CLOCKS_PER_SEC) << "s" << std::endl;

}

void Application::PerlinSimplexBenchmark()
/*-------------------------------------------------------------------------*\
|	Purpose:	Compare Perlin SIMD and Linear Noise with Linear Simplex	|
|				noise														|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	Sends times to Console Out									|
|																			|
\*-------------------------------------------------------------------------*/
{
	float out = 0;
	double myTimer;
	const int itnum = 10000;

	NoiseGenerator thingy;

	std::cout << "Generating Noise over " << itnum << "x" << itnum << " range with 12 Octaves" << std::endl;

	float maxAmp = thingy.MaxAmplitude(myNoise);
	myTimer = clock();
	for (int i = 0; i < itnum; i++) {
		for (int j = 0; j < itnum; j++) {
			out += (thingy.SIMDPerlin2D(i, j, myNoise)/maxAmp)*myNoise.amplitude;
		}
	}

	myTimer = clock() - myTimer;
	std::cout << std::endl << "SIMD Perlin Generation Time: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;
	float out2 = 0;
	myTimer = clock();

	for (int i = 0; i < itnum; i++) {
		for (int j = 0; j < itnum; j++) {
			out2 += thingy.Perlin2D(i, j, myNoise);
		}
	}
	myTimer = clock() - myTimer;
	std::cout << std::endl << "SISD Perlin Generation Time: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;
	float sum = 0;

	sum = abs(out2 - out);


	sum /= (itnum*itnum);
	std::cout << std::endl << "Average Difference: " << sum << std::endl;

	out2 = 0;

	myTimer = clock();
	for (int i = 0; i < itnum; i++) {
		for (int j = 0; j < itnum; j++) {
			out2 += thingy.FractalSimplex(i, j, myNoise);
		}
	}
	myTimer = clock()-myTimer;

	std::cout << std::endl << "SISD Simplex Generation Time: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;
	sum = abs(out2 - out);
	sum /= (itnum*itnum);
	//  Force the compiler to do the operation and not optimise it out
	std::cout << "This comparison Makes no sense!" << sum << std::endl;
}

void Application::SIMDThreadingBenchmark()
/*-------------------------------------------------------------------------*\
|	Purpose:	Compare Heightmap generation times using linear execution,	|
|				simd execution and threaded executions						|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	Sends times to Console out.									|
|																			|
\*-------------------------------------------------------------------------*/
{
	float out = 0;
	double myTimer;
	const int itnum = 10000;

	NoiseGenerator thingy;
	Heightmap heights;

	float maxAmp = thingy.MaxAmplitude(myNoise);
	myTimer = clock();
	heights.GenHeightsLinear(0,0,myNoise,5000);
	myTimer = clock() - myTimer;
	std::cout << std::endl << "SISD Perlin Generation Time: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

	myTimer = clock();
	heights.GenHeightsSIMD(0,0,myNoise,5000);
	myTimer = clock() - myTimer;
	std::cout << std::endl << "SIMD Perlin Generation Time: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

	myTimer = clock();
	heights.TBBGenerateHeightField(0,0,myNoise,5000);
	myTimer = clock() - myTimer;
	std::cout << std::endl << "MIMD Perlin Generation Time: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

	myTimer = clock();
	heights.GenHeightsTBBSIMD(0,0,myNoise,5000);
	myTimer = clock() - myTimer;
	std::cout << std::endl << "SIMD+MIMD Perlin Generation Time: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

}

