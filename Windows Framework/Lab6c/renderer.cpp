#include "renderer.h"

#include "gameobject.h"
#include "terrainmanager.h"

#include <iostream>
#include <fstream>
#include <sstream>

CameraModule* RenderManager::activeCamera;

RenderManager::RenderManager():
	sun(LightSource::DIRECTIONAL)
{
	opaqueRenderList.reserve(2048);
	sun.SetAmbient(0.15f);
	sun.SetColour(0.7f, 0.7f, 0.7f);
	sun.RotateDeltaX(90);
	elapsed = 0.0f;
}

RenderManager::~RenderManager(void)
{
}

void RenderManager::MaintainRenderList()
//  Ensure the renderList is sorted in the right order
{
	renderList.sort(RenderManager::MeshComparator);
}

bool RenderManager::AddToRenderer(Mesh &m)
//  Adds specified mesh to the render list
//  If the mesh is brand new it will import the texture and compile the
//  display list needed to draw the mesh, if the mesh or texture has been
//  Used before they will not be reimported/compiled and we will instead use
//  the old versions
{
	std::string meshModel = m.GetMeshSourceFilePath();

	//  Only bother compiling a new Display List if one doesn't already exist for this object
	if (!VAOMap.count(m.GetUniqueID())) {
		if (meshModel.length() > 0) {
			if (!MeshFileMap.count(meshModel)) {
				//  Compile the object's Display List and remember that this object has been compiled to save repetition
				MeshFileMap[meshModel] = SetupVAO(m);
			}
			VAOMap[m.GetUniqueID()] = MeshFileMap[meshModel];
		} else {
			VAOMap[m.GetUniqueID()] = SetupVAO(m);
		}
	}

	//  Add our new (or repeated) item to the render list
	if(m.IsTransparent()) {
		renderList.push_back(m.GetUniqueID());
	} else {
		opaqueRenderList.push_back(m.GetUniqueID());
	}
//	m.DeleteVertexData();
	return true;
}

void RenderManager::AddLight(LightSource &l)
{
	if (lightObjects.size() < MAXLIGHTS) {
		lightObjects.push_back(l);
	}
}

void RenderManager::AddSkyBox(Mesh &m)
{
	skyBox = SetupVAO(m);
	sky = m.GetUniqueID();
}

void RenderManager::AddTerrainToRenderer(TerrainManager &t)
{
	terrain = SetupVAO(t.GetTerrainMesh());
	terrainSize = t.GetTerrainMesh().GetSize();
	terrainManager = &t;
	water = SetupVAO(t.GetWaterMesh());
}

void RenderManager::RemoveFromRenderer(Mesh m)
//  This function removes a specified mesh from the scene
//  It will not delete the associated texture or display list
{
	if (m.IsTransparent()) {
		std::list<int>::iterator it;
		//  Check through list to find mesh we want to remove
		for(it = renderList.begin(); *it != m.GetUniqueID(); it++) {}
		//  remove it
		renderList.erase(it);
	} else {
		std::vector<int>::iterator it;
		for(it = opaqueRenderList.begin(); *it != m.GetUniqueID(); it++) {}
		//  remove it
		opaqueRenderList.erase(it);
	}
}

void RenderManager::RenderAll()
//  Iterates through all the items in the render list
//  translates the render context to the object's base position then
//  renders the object.
//  Objects are drawn in order, opaque first then transparent, with transparent objects
//  further away being drawn first
{
	BuildProjectionMatrix();
	PrepareLights();
	viewMatrixMade = false;

	DrawSkyBox();
	DrawTerrain();

	std::vector<int>::iterator vit;
	if (opaqueRenderList.size() > 0) {
		for (vit = opaqueRenderList.begin(); vit != opaqueRenderList.end(); vit++) {
			if (!DrawMesh(*vit)) {
				opaqueRenderList.erase(vit);
			}
		}
	}

	DrawWater();

	std::list<int>::iterator lit;
	if (renderList.size() > 0) {
		for (lit = renderList.begin(); lit != renderList.end(); lit++) {
			if (!DrawMesh(*lit)) {
				renderList.erase(lit);
			}
		}
	}
}

GLuint RenderManager::SetupVAO(Mesh &m)
{

	//  Create a VertexArrayObject (like a display list for buffer objects) and set it to current
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//  Make our vertex buffer, pass in the vertex data
	GLuint vertBuffer;
	glGenBuffers(1, &vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, m.GetSizeOfVerts(), m.GetVertexArrayBase(), GL_STATIC_DRAW);
	

	//  Make our normal buffer, pass in the normals
	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m.GetSizeOfNormals(), m.GetNormalArrayBase(), GL_STATIC_DRAW);

	//  Make our UV buffer, pass in the UV coords
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m.GetSizeOfUVs(), m.GetUVArrayBase(), GL_STATIC_DRAW);

	//  bind the vertex buffer to location 0 (layout(location = 0)) in the shaders
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//  bind the normal buffer to location 1 (layout(location = 1)) in the shaders
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//  bind the vertex buffer to location 2 (layout(location = 2)) in the shaders
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//  Bind our index array to it's own buffer.
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.GetIndexLength()*sizeof(unsigned int), m.GetIndexArrayBase(), GL_STATIC_DRAW);

	//  Unbind our VAO so we don't mess with it
	glBindVertexArray(0);

	//  Pass the reference out to be used later
	return vao;
}

bool RenderManager::MeshComparator(int rhs, int lhs)
//  A function for ordering the renderList
//  Places all NON-TRANSPARENT meshes at the start of the list, generally unordered
//  And places TRANSPARENT meshes at the end of the list, ordered by distance from
//  primary camera
{

	//  If both have transparency then find their position relative to the camera and sort based on distance, furthest first
	if(Mesh::GetMeshPointer(rhs)->IsTransparent() == true && Mesh::GetMeshPointer(lhs)->IsTransparent() == true) {
		Vector4 rhsDist = Mesh::GetMeshPointer(rhs)->GetParentPointer()->GetPosition() - activeCamera->GetParent()->GetPosition();
		Vector4 lhsDist = Mesh::GetMeshPointer(lhs)->GetParentPointer()->GetPosition() - activeCamera->GetParent()->GetPosition();

		return rhsDist.LengthSqr() > lhsDist.LengthSqr();
	//  If only one is transparent then place NON-TRANSPARENT one first
	} else if (Mesh::GetMeshPointer(rhs)->IsTransparent() == true || Mesh::GetMeshPointer(lhs)->IsTransparent() == true) {
		//  If rhs is transparent (true) it needs to go last, so false.  If rhs isn't transparent (false) then it needs to go first, so true
		return !Mesh::GetMeshPointer(rhs)->IsTransparent();
	} else {
		//  If both are NON-TRANSPARENT then order doesn't matter
		return true;
	}
}


Matrix4x4 RenderManager::BuildModelMatrix(GameObject g)
{
	Matrix4x4 translation(Matrix4x4::IDENTITY);
	Matrix4x4 rotation(Matrix4x4::IDENTITY);
	Matrix4x4 scale(Matrix4x4::IDENTITY);

	translation.Translation(g.GetPosition());
	rotation = g.GetRotation().GetRotationMatrix();
	scale.Scale(g.GetScale());
	Matrix4x4 result = scale * rotation * translation;

	ConvertToOpenGLMatrix(result, modelMatrix);
	return result;
}

Matrix4x4 RenderManager::BuildViewMatrix()
{
	Matrix4x4 view;
	if (viewMatrixMade == false) {
		Vector4 position = activeCamera->GetParent()->GetPosition();
		Vector4 upVector = activeCamera->GetParent()->GetLocalY();
		Vector4 lookAt = position + activeCamera->GetParent()->GetLocalZ();

		view.LookAt(position, lookAt, upVector);
		ConvertToOpenGLMatrix(view, viewMatrix);
		viewMat = view;
		viewMatrixMade = true;
	} else {
		view = viewMat;
	}
	return view;
}

void RenderManager::BuildModelViewMatrix(GameObject g)
{
	Matrix4x4 mvMatrix;
	Matrix4x4 model = BuildModelMatrix(g);
	Matrix4x4 view = BuildViewMatrix();


	mvMatrix = model * view;
	ConvertToOpenGLMatrix(mvMatrix, modelViewMatrix);


	float mv[3][3];
	mv[0][0] = mvMatrix(0,0);
	mv[1][1] = mvMatrix(1,1);
	mv[2][2] = mvMatrix(2,2);
	mv[1][0] = mvMatrix(0,1);
	mv[2][0] = mvMatrix(0,2);
	mv[2][1] = mvMatrix(1,2);
	mv[0][1] = mvMatrix(1,0);
	mv[0][2] = mvMatrix(2,0);
	mv[1][2] = mvMatrix(2,1);

	float det = mv[0][0] * (mv[1][1]*mv[2][2] - mv[1][2]*mv[2][1]) - mv[0][1] * (mv[1][0] * mv[2][2] - mv[1][2] * mv[2][0]) + mv[0][2] * (mv[1][0] * mv[2][1] - mv[1][1] * mv[2][0]);

	mv[0][0] = (mv[1][1]*mv[2][2] - mv[1][2]*mv[2][1])/det;
	mv[0][1] = -(mv[1][0]*mv[2][2] - mv[1][2]*mv[2][0])/det;
	mv[0][2] = (mv[1][0]*mv[2][1] - mv[1][1]*mv[2][0])/det;
	mv[1][0] = -(mv[0][1]*mv[2][2] - mv[0][2]*mv[2][1])/det;
	mv[1][1] = (mv[0][0]*mv[2][2] - mv[0][2]*mv[2][0])/det;
	mv[1][2] = -(mv[0][0]*mv[2][1] - mv[0][1]*mv[2][0])/det;
	mv[2][0] = (mv[0][1]*mv[1][2] - mv[0][2]*mv[1][1])/det;
	mv[2][1] = -(mv[0][0]*mv[1][2] - mv[0][2]*mv[1][0])/det;
	mv[2][2] = (mv[0][0]*mv[1][1] - mv[0][1]*mv[1][0])/det;

	Matrix4x4 normMatrix(Matrix4x4::IDENTITY);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			normMatrix.elem[i][j] = mv[j][i];
		}
	}

	ConvertToOpenGLMatrix(normMatrix, normalMatrix);
}

void RenderManager::BuildSkyBoxViewMatrix(GameObject g)
{
	g.SetRotate(0.0f, 0.0f, 0.0f);
	Matrix4x4 mat;
	mat = BuildModelMatrix(g) * BuildViewMatrix();
	ConvertToOpenGLMatrix(mat, modelViewMatrix);
}

void RenderManager::BuildProjectionMatrix()
{
	Matrix4x4 projection;
	projection.Projection(activeCamera->GetFieldOfView(), (GLfloat)activeCamera->GetWindowWidth()/(GLfloat)activeCamera->GetWindowHeight(), activeCamera->GetNearClipPlane(), activeCamera->GetFarClipPlane());
	ConvertToOpenGLMatrix(projection, projectionMatrix);
}

void RenderManager::ConvertToOpenGLMatrix(Matrix4x4 m, GLfloat* target)
{
	target[0] = m.elem[0][0];
	target[1] = m.elem[0][1];
	target[2] = m.elem[0][2];
	target[3] = m.elem[0][3];
	target[4] = m.elem[1][0];
	target[5] = m.elem[1][1];
	target[6] = m.elem[1][2];
	target[7] = m.elem[1][3];
	target[8] = m.elem[2][0];
	target[9] = m.elem[2][1];
	target[10] = m.elem[2][2];
	target[11] = m.elem[2][3];
	target[12] = m.elem[3][0];
	target[13] = m.elem[3][1];
	target[14] = m.elem[3][2];
	target[15] = m.elem[3][3];
}

void RenderManager::DrawSkyBox()
{
	Mesh* m = Mesh::GetMeshPointer(sky);

	Material mat = m->GetMaterial();
	UniformLocations uniforms = mat.GetUniforms();
	mat.Apply();

	//  Build the modelview matrix for the mesh
	BuildSkyBoxViewMatrix(*activeCamera->GetParent());

	//  Find the uniform locations for this program and put relevant data into said locations
	SetUniforms(uniforms);

	//  Bind the VAO and draw the array
	glBindVertexArray(skyBox);
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES,  m->GetNumberOfVerts(), GL_UNSIGNED_INT, (void*)0);
	glEnable(GL_DEPTH_TEST);

	//  unbind our shaders and arrays
	glBindVertexArray(0);
	glUseProgram(0);
}

void RenderManager::DrawTerrain()
{
	float xmul = 0;
	float ymul = 0;

	Vector2 b = terrainManager->GetBase(0, 0);
	Vector2 b2 = terrainManager->GetBase(1, 1);
	float dif = b.u - b2.u;


	for (int i = 0; i < terrainManager->GetSize(); i++) {

		for (int j = 0; j < terrainManager->GetSize(); j++) {
			Material mat = terrainManager->GetMaterial(i, j);
			UniformLocations uniforms = mat.GetUniforms();
			mat.Apply();

			xmul = terrainManager->GetBase(i, j).u;
			ymul = terrainManager->GetBase(i, j).v;

			GameObject base;
			base.SetLocation(xmul, 0.0f, ymul);

			//  Build the modelview matrix for the mesh
			BuildModelViewMatrix(base);

			//  Find the uniform locations for this program and put relevant data into said locations
			SetUniforms(uniforms);	
			glUniform1f(uniforms.XMulFactor, xmul);
			glUniform1f(uniforms.YMulFactor, ymul);


			//  Bind the VAO and draw the array
			glBindVertexArray(terrain);

			glPatchParameteri(GL_PATCH_VERTICES, 3);
			glDrawElements(GL_PATCHES, terrainManager->GetTerrainMesh().GetIndexLength(), GL_UNSIGNED_INT, (void*)0);

			//  unbind our shaders and arrays
			glBindVertexArray(0);
			glUseProgram(0);

		}

	}
}

void RenderManager::DrawWater()
{

	float xmul = 0;
	float ymul = 0;
	Vector2 b = terrainManager->GetBase(0, 0);
	Vector2 b2 = terrainManager->GetBase(1, 1);
	float dif = b.u - b2.u;

	for (int i = 0; i < terrainManager->GetSize(); i++) {
		for (int j = 0; j < terrainManager->GetSize(); j++) {
			xmul = terrainManager->GetBase(i, j).u;
			ymul = terrainManager->GetBase(i, j).v;

			Material mat = terrainManager->GetWaterMaterial(i, j);
			UniformLocations uniforms = mat.GetUniforms();
			mat.Apply();
			//  Build the modelview matrix for the mesh
			GameObject base;
			base.SetLocation(xmul, 0.0f, ymul);
			BuildModelViewMatrix(base);

			//  Find the uniform locations for this program and put relevant data into said locations
			SetUniforms(uniforms);
			glUniform1f(uniforms.Time, elapsed);
			glUniform1f(uniforms.XMulFactor, xmul);
			glUniform1f(uniforms.YMulFactor, ymul);

			//  Bind the VAO and draw the array
			glBindVertexArray(water);

			glPatchParameteri(GL_PATCH_VERTICES, 3);
			glDrawElements(GL_PATCHES, terrainManager->GetWaterMesh().GetIndexLength(), GL_UNSIGNED_INT, (void*)0);

			//  unbind our shaders and arrays
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}
	elapsed += 0.1f;

}

bool RenderManager::DrawMesh(int meshID)
{
	Mesh* m = Mesh::GetMeshPointer(meshID);

	if (m != NULL) {
		Material mat = m->GetMaterial();
		UniformLocations uniforms = mat.GetUniforms();
		mat.Apply();

		//  Build the modelview matrix for the mesh
		GameObject* g = m->GetParentPointer();

		BuildModelViewMatrix(*g);
		
		//  Find the uniform locations for this program and put relevant data into said locations
		SetUniforms(uniforms);

		//  Bind the VAO and draw the array
		glBindVertexArray(VAOMap[meshID]);

		glDrawElements(GL_TRIANGLES, m->GetNumberOfVerts(), GL_UNSIGNED_INT, (void*)0);

		//  unbind our shaders and arrays
		glBindVertexArray(0);
		glUseProgram(0);
		return true;
	} else {
		//  If the mesh pointer is NULL then retrun false so we know to remove it from the renderlist
		return false;
	}
}

void RenderManager::PrepareLights()
{
	std::vector<LightSource>::iterator it;
	int i = 0;
	for (it = lightObjects.begin(); it != lightObjects.end(); it++) {
		lights[i] = it->GetLightAsStruct(BuildViewMatrix());
		i++;
	}
}


void RenderManager::SetUniforms(UniformLocations uniform)
//  Send the lights and matrices to the current shader program
{
	glUniformMatrix4fv(uniform.ProjectionMatrix, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(uniform.ModelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(uniform.ViewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(uniform.ModelViewMatrix, 1, GL_FALSE, modelViewMatrix);
	glUniformMatrix4fv(uniform.NormalMatrix, 1, GL_FALSE, normalMatrix);
	glUniform1f(uniform.MapWidth, terrainSize);
	glUniform1f(uniform.Magnitude, 80);

	for (int i = 0; i < lightObjects.size(); i++) {
		glUniform4fv(uniform.LightColours[i], 1, lights[i].colour);
		glUniform4fv(uniform.LightPositions[i], 1, lights[i].position);
		glUniform1i(uniform.LightTypes[i], lights[i].type);
	}

	glUniform1i(uniform.NumLights, lightObjects.size());
}
