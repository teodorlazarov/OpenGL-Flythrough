#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

#include "Vector3D.h"
#include "BezierSpline.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model house;
Model building;
Model building1;
Model building2;
Model streetLamp;
Model streetLamp1;
Model liberty;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

BezierSpline* bezierSpline;
BezierSpline* lookSpline;


// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObject()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	 x		y	  z		u	  v     nx    ny    nz
			-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 0.0f,  1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0, 0.0f, 10.0f,		0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObject();
	CreateShaders();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	building = Model();
	building.LoadModel("Models/Building_A01.obj");

	building1 = Model();
	building1.LoadModel("Models/Building_A02.obj");

	building2 = Model();
	building2.LoadModel("Models/Building_A06.obj");

	streetLamp = Model();
	streetLamp.LoadModel("Models/StreetLamp.obj");

	streetLamp1 = Model();
	streetLamp1.LoadModel("Models/streetlamp1.obj");

	liberty = Model();
	liberty.LoadModel("Models/LibertStatue.obj");


	mainLight = DirectionalLight(1.0f, 0.7f, 0.01f,
		0.2f, 0.1f,
		0.0f, -0.2f, -1.0f);

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.1f,
		1.0f, -1.0f, 1.3f,
		0.3f, 0.1f, 0.1f);

	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 0.5f,
		0.0f, 2.0f,
		-2.0f, -1.5f, 0.3f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	bezierSpline = new BezierSpline();
	bezierSpline->addPoint(Vector3D(5, 2, 10));
	bezierSpline->addPoint(Vector3D(5, 1, 5));
	bezierSpline->addPoint(Vector3D(2.5, 0, 3));
	bezierSpline->addPoint(Vector3D(2, 0, 2));
	bezierSpline->addPoint(Vector3D(2, 0, 1));
	bezierSpline->addPoint(Vector3D(2, 0, -0.5));
	bezierSpline->addPoint(Vector3D(2, 0, -3));
	bezierSpline->addPoint(Vector3D(0, -1, -3));
	bezierSpline->addPoint(Vector3D(0, -1, -2));
	bezierSpline->addPoint(Vector3D(-2, -1, -1));
	bezierSpline->addPoint(Vector3D(-2, 0, 0));
	bezierSpline->addPoint(Vector3D(-2.5, 0, 1));
	bezierSpline->addPoint(Vector3D(-2.0, 0, 3));
	bezierSpline->addPoint(Vector3D(-1.0, 1, 4));
	bezierSpline->addPoint(Vector3D(2, 1, 7));
	bezierSpline->addPoint(Vector3D(5, 2, 10));
	

	lookSpline = new BezierSpline();
	lookSpline->addPoint(Vector3D(0, -0.1, -1));
	lookSpline->addPoint(Vector3D(-1, 0, 0));
	lookSpline->addPoint(Vector3D(0.1, -0.2, 1));
	lookSpline->addPoint(Vector3D(0, -0.1, -1));


	float t = 0.0f;

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.setPosition(bezierSpline->deCasteljau(t));
		camera.setFront(lookSpline->deCasteljau(t));

		if (t >= 1)
		{
			t = 0.0f;
		}
		t += 0.0001f;

		//camera.keyControl(mainWindow.getKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChanged(), mainWindow.getYChanged());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model;

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
	
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		building.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.5f, -2.0f, -0.5f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		building1.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		building2.RenderModel();
		
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		streetLamp.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, -2.0f, 1.3f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		streetLamp1.RenderModel();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.5f, -2.0f, 1.7f));
		model = glm::rotate(model, 160.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		liberty.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}