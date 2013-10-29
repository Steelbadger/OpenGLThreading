#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	The Renderer.  This class receives references to ALL meshes	//
//	, checks their information (transparent/nontransparent),	//
//	enrolls them in the correct list (an unsorted Opaque render	//
//	list and a sorted-by-distance transparent list)				//
//	This class also builds the model-view and projection		//
//	matrices each frame (using a reference to the current		//
//	camera) and passes them to the shader programs				//
//////////////////////////////////////////////////////////////////
#include "cameramodule.h"
#include "defines.h"
#include "glew.h"
#include "lights.h"
#include "material.h"
#include "mesh.h"
#include "mywindow.h"
#include "staticobject.h"
#include "terrain.h"
#include "water.h"

#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>

class TerrainManager;

class RenderManager
{
public:
	RenderManager();
	~RenderManager(void);

	//  Sort the transparent list
	void MaintainRenderList();

	//  Change the camera (used for view and projection matrices)
	void SetActiveCamera(CameraModule& cam){activeCamera = &cam;}
	//  Allows fetching of the state of the camera via it's parent GameObject
	GameObject* GetCameraParent(){return activeCamera->GetParent();}

	//  Add a mesh to the render list
	bool AddToRenderer(Mesh &m);
	//  Remove the mesh from the render list
	void RemoveFromRenderer(Mesh m);

	//  Add a mesh to the renderer as a skybox (special case, automatically uses
	//	the camera as the position and disables the depth test while drawing)
	void AddSkyBox(Mesh &m);

	//  Give the renderer access to the terrain manager so we can
	//  find the properties of the every-changing terrain
	void AddTerrainToRenderer(TerrainManager &t);

	//  Derp.  Add a light source to the renderer
	void AddLight(LightSource &l);

	//  Render the scene
	void RenderAll();

	//  Function used to sort the (transparent) meshes
	static bool MeshComparator(int rhs, int lhs);

private:
	
	//  Pointer to the current camera from which the scene is being drawn
	static CameraModule* activeCamera;

	//  Create the Vertex Array Object for the mesh
	GLuint SetupVAO(Mesh &m);

	//  Do some OpenGL setup
	void InitializeOpenGL();

	Matrix4x4 BuildModelMatrix(GameObject g);
	Matrix4x4 BuildViewMatrix();
	void BuildProjectionMatrix();
	void BuildModelViewMatrix(GameObject g);
	void BuildSkyBoxViewMatrix(GameObject g);

	//  Boolean so that we don't make the view and projection matrices multiple times per frame
	bool viewMatrixMade;

	//  Boolean that tracks if our lights have been exported to the structs used to update shader uniforms
	bool lightsPrepared;

	//  Export lights to structs and store
	void PrepareLights();

	//  Send matrices and lights to the current shader program
	void SetUniforms(UniformLocations l);
	
	//  Draw the mesh stored as the skybox
	void DrawSkyBox();

	//  Draw all the terrain chunks
	void DrawTerrain();
	//  Draw a mesh designated by meshID
	bool DrawMesh(int meshID);
	//  Draw all the chunks of water
	void DrawWater();
	//  Time variable for passing to the water shader (for waves)
	float elapsed;

	//  Take a matrix4x4 and rewrite it as a 1d array in OpenGL form
	void ConvertToOpenGLMatrix(Matrix4x4 m, GLfloat* target);

	//  Transparent meshes in this list (list for cheaper sorting)
	std::list<int> renderList;
	//  Opaque meshes in this list (vector as no need to sort)
	std::vector<int> opaqueRenderList;


	//  The arrays we use to pass the arrays to the shaders
	GLfloat modelMatrix[16];
	GLfloat viewMatrix[16];
	Matrix4x4 viewMat;
	GLfloat modelViewMatrix[16];
	GLfloat projectionMatrix[16];
	GLfloat normalMatrix[16];

	//  The skybox VAO
	GLuint skyBox;
	//  The skybox mesh
	int sky;

	//  The terrain VAO
	GLuint terrain;
	//  The terrain can be fetched from here
	TerrainManager* terrainManager;

	//  Some data to aid in terrain drawing (shaders need to know the size
	//  of the chunks of terrain)
	float terrainSize;

	//  The water VAO
	GLuint water;

	//  The sun
	LightSource sun;
	Light sunSource;

	//  Other lights
	std::vector<LightSource> lightObjects;
	Light lights[MAXLIGHTS];

	//  Maps for looking up VAOs
	std::map<int, GLuint> VAOMap;
	std::map<std::string, GLuint> MeshFileMap;
};
