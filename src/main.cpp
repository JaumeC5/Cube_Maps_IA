// Std. Includes
#include <string>
#include <algorithm>
using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camara.h"
#include "Model.h"
#include "Object.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>

// GLM Mathemtics
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>


using namespace std;
const GLint screenWidth = 800, screenHeight = 600;
bool WIDEFRAME = false;
GLsizei numBuffers = 3;
float switcher = 0.0;
float angleX = 0.0;
float angleY = 0.0;
bool rotateLeft, rotateRight, rotateUp, rotateDown;
float aspectRatio = screenWidth / screenHeight;
float timeNow, lastTime, deltaTime;
bool firstMouse = true;
GLfloat lastX = 400, lastY = 300;
bool mo1, mo2, mo3 = false;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement(GLFWwindow *window);
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(vector<const GLchar*> faces);

// Camera
Camara camera(glm::vec3(0.0f, 0.0f, 3.f), glm::normalize(glm::vec3(0.f, 0.f, 3.f) - glm::vec3(0.f, 0.f, 0.f)), 0.1, 60);
bool keys[1024];

GLfloat lastFrame = 0.0f;

glm::vec3 lightColor = glm::vec3(0.6f, 0.0f, 0.5);

Camara cam(glm::vec3(0.0f, 0.0f, 3.f), glm::normalize(glm::vec3(0.f, 0.f, 3.f) - glm::vec3(0.f, 0.f, 0.f)), 0.1, 60);

glm::vec3 cubRot = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 cubScal = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 cubPos = glm::vec3(0.f, 0.f, 0.f);

int main()//fgh
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Salt begins", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Setup and compile our shaders
	Shader shader("./src/advanced.vs", "./src/advanced.frag");
	Shader skyboxShader("./src/skybox.vs", "./src/skybox.frag");

	Shader squareShader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader textureShader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");
	Shader cubeShader("./src/cubeVertex.v", "./src/cubeFragment.f");
	Shader modelShader("./src/modelVertex.v", "./src/modelFrag.f");
	Shader miniCubo1("./src/minicube1v.v", "./src/minicube1f.f");

	//Material
	Shader materialShader("./src/material.v", "./src/material.f");

	//Material y textura
	Shader niceCubem8("./src/textMat.v", "./src/textMat.f");

	//Direcional
	Shader lightShader("./src/vertexLC.v", "./src/fragLC.f");

	//Puntual
	Shader pointShader("./src/pointLight.v", "./src/pointLight.f");

	//Focal
	Shader spotShader("./src/spotLight.v", "./src/spotLight.f");

	//Multilight
	Shader multiShader("./src/multilight.v", "./src/multilight.f");

	glm::vec3 lightPositions[] = {
		glm::vec3(1.f,  1.f,  0.0f),
		glm::vec3(2.f, 1.f, 0.0f),
		glm::vec3(3.0f,  1.0f, 0.0f),
		glm::vec3(4.0f,  1.0f, 0.0f),
		glm::vec3(5.0f,  1.0f, 0.0f)

	};

	Object bigC(cubScal, cubRot, cubPos, cube);
	Object miniCube(glm::vec3(1.f, 0.1f, 0.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), cube);
	Object pointCube1(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), cube);
	Object pointCube2(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), cube);
	Object spotCube1(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), cube);
	Object spotCube2(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), cube);
	Object directional(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), cube);

	Object bigCube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), cube);
	Object transp(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.f, 1.f, 1.f), cube);
	//Models 3d
#if(false)
	Model m1("./Models/m1/nanosuit.obj");
	Model m2("./Models/m2/eagle.obj");
	Model m3("./Models/m3/dog.obj");
#endif
	// Definir el buffer de vertices

	//cuadrado
	GLfloat vertexBufferObject[] = {

		0.5f, 0.5f, 0.f, //  0. top right
		0.5f, -0.5f, 0.f,//   1. bot right
		-0.5f, -0.5f, 0.f,//    2. bot left
		-0.5f, 0.5f, 0.f, //  3. top left

	};

	//textura
	GLfloat vertices[] = {
		//  Position      Color             Texcoords
		-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
		0.5f,  0.5f,0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
	};

	GLfloat textureBufferObject[] = {

		0.5f, 0.5f, 0.f, //  0. top right
		0.5f, -0.5f, 0.f,//   1. bot right
		-0.5f, -0.5f, 0.f,//    2. bot left
		-0.5f, 0.5f, 0.f, //  3. top left

	};

	//cubo simple
	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 CubesPositionBuffer[] = {
		glm::vec3(0.0f ,  0.0f,  0.0f),
		glm::vec3(2.0f ,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f , -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f , -2.0f, -2.5f),
		glm::vec3(1.5f ,  2.0f, -2.5f),
		glm::vec3(1.5f ,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

#pragma region "object_initialization"
	GLfloat cubeVertices[] = {
		// Positions          // Normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// Setup cube VAO
	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);


	// Load textures
	GLuint cubeTexture = loadTexture("./src/gr8.jpg");

	// Cargar texturas, cubo con luz
	GLuint diffuseMap, specularMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	int width, height;
	unsigned char* image;
	// Diffuse map
	image = SOIL_load_image("./src/sapphire.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Specular map
	image = SOIL_load_image("./src/sapphire_specular.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

#pragma endregion

	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("./src/right.jpg");
	faces.push_back("./src/left.jpg");
	faces.push_back("./src/top.jpg");
	faces.push_back("./src/bottom.jpg");
	faces.push_back("./src/back.jpg");
	faces.push_back("./src/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);

	//ordre dels vertex dels triangles del quadrat
	GLuint indexBufferObject[] = {
		0, 1, 3,
		1, 2, 3
	};

	//Inicializar objetos
	bigC.Initiate();
	miniCube.Initiate();
	pointCube1.Initiate();
	pointCube2.Initiate();
	spotCube1.Initiate();
	spotCube2.Initiate();
	directional.Initiate();
	bigCube.Initiate();
	transp.Initiate();

	GLint variableShader = glGetUniformLocation(squareShader.Program, "Sion"); //enllaçar amb variable dins els " "
	GLint imgSwitcher = glGetUniformLocation(textureShader.Program, "alternador");
	GLint uniTrans = glGetUniformLocation(textureShader.Program, "trans");
	GLint uniView = glGetUniformLocation(textureShader.Program, "view");
	GLint uniProj = glGetUniformLocation(textureShader.Program, "proj");

	//	GLint uniModel = glGetUniformLocation(lightShader.Program, "model");
	GLint uniView2 = glGetUniformLocation(lightShader.Program, "view");
	GLint uniProj2 = glGetUniformLocation(lightShader.Program, "proj");

	GLint uniView3 = glGetUniformLocation(miniCubo1.Program, "view");
	GLint uniProj3 = glGetUniformLocation(miniCubo1.Program, "proj");

	GLint uniView4 = glGetUniformLocation(materialShader.Program, "view");
	GLint uniProj4 = glGetUniformLocation(materialShader.Program, "proj");

	GLint uniView5 = glGetUniformLocation(niceCubem8.Program, "view");
	GLint uniProj5 = glGetUniformLocation(niceCubem8.Program, "proj");

	GLint uniView6 = glGetUniformLocation(pointShader.Program, "view");
	GLint uniProj6 = glGetUniformLocation(pointShader.Program, "proj");

	GLint uniView7 = glGetUniformLocation(spotShader.Program, "view");
	GLint uniProj7 = glGetUniformLocation(spotShader.Program, "proj");
	GLint lightSpotCutOffLoc = glGetUniformLocation(spotShader.Program, "light.cutOff");
	GLint lightSpotOuterCutOffLoc = glGetUniformLocation(spotShader.Program, "light.outerCutOff");
	GLint lightSpotdirLoc = glGetUniformLocation(spotShader.Program, "light.direction");

	GLint uniView8 = glGetUniformLocation(multiShader.Program, "view");
	GLint uniProj8 = glGetUniformLocation(multiShader.Program, "proj");

	//Matrius
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 proj;
	GLfloat radio = 8.0f;

	glm::mat4 view;
	glEnable(GL_DEPTH_TEST);

	//Raton
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Inicializar algunos parámetros de los cubos
	bigC.Scale(cubScal);
	miniCube.Move(glm::vec3(-2.f, 2.f, -2.0f));
	miniCube.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	spotCube1.Move(lightPositions[0]);
	spotCube1.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	pointCube1.Move(lightPositions[1]);
	pointCube1.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	spotCube2.Move(lightPositions[2]);
	spotCube2.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	pointCube2.Move(lightPositions[3]);
	pointCube2.Scale(glm::vec3(0.1f, 0.1f, 0.1f));


	//directional.Move(glm::vec3(-2.f, 2.1f, -2f));
	directional.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	bigCube.Move(glm::vec3(3.f, 1.f, 0.f));
	bigCube.Scale(glm::vec3(5.f, 5.f, 5.f));
	transp.Move(glm::vec3(0.f, 0.f, 0.f));
	// Draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement(window);

		// Clear buffers
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw scene as normal
		
		proj = glm::perspective(camera.GetFOV(), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		glDepthMask(GL_FALSE);  // Change depth function so depth test passes when values are equal to depth buffer's content
		
		skyboxShader.USE();
		
		view = glm::mat4(glm::mat3(camera.LookAt()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// skybox 
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);		
		glUniform3f(glGetUniformLocation(skyboxShader.Program, "cameraPos"), camera.camPos.x, camera.camPos.y, camera.camPos.z);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE); 
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		view = camera.LookAt();


		//Transparent Cube
		shader.USE();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(shader.Program, "cameraPos"), camera.camPos.x, camera.camPos.y, camera.camPos.z);

		//glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		multiShader.USE();
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		glUniform3f(glGetUniformLocation(multiShader.Program, "viewPos"), cam.camPos.x, cam.camPos.y, cam.camPos.z);
		//directional
		glUniform3f(glGetUniformLocation(multiShader.Program, "dirLight.direction"), -2.f, 5.f, -2.f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		// Point light 1
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[0].position"), lightPositions[0].x, lightPositions[0].y, lightPositions[0].z);
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "pointLights[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(multiShader.Program, "pointLights[0].quadratic"), 0.032);
		// Point light 2
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[1].position"), lightPositions[1].x, lightPositions[1].y, lightPositions[1].z);
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "pointLights[1].linear"), 0.09);
		glUniform1f(glGetUniformLocation(multiShader.Program, "pointLights[1].quadratic"), 0.032);

		// SpotLight 1
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[0].position"), lightPositions[2].x, lightPositions[2].y, lightPositions[2].z);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[0].direction"), 0.f, -1.f, 0.f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[0].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[0].linear"), 0.09);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[0].quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[0].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight 2
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[1].position"), lightPositions[3].x, lightPositions[3].y, lightPositions[3].z);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[1].direction"), 0.f, -1.f, 0.f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[1].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "spotLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[1].linear"), 0.09);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[1].quadratic"), 0.032);
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[1].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(multiShader.Program, "spotLights[1].outerCutOff"), glm::cos(glm::radians(15.0f)));

		glUniform1f(glGetUniformLocation(multiShader.Program, "material.shininess"), 32.f);
		glUniform1i(glGetUniformLocation(multiShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(multiShader.Program, "material.specular"), 1);

		glUniformMatrix4fv(uniView8, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj8, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(glGetUniformLocation(multiShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(bigC.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel
		//activar texturas cubo de luz
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		bigC.Move(cubPos);
		bigC.Rotate(cubRot);

		bigC.Draw();
		bigCube.Draw();

		miniCubo1.USE();

		glUniformMatrix4fv(glGetUniformLocation(miniCubo1.Program, "model"), 1, GL_FALSE, glm::value_ptr(miniCube.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniView3, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj3, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		miniCube.Draw();

		glUniformMatrix4fv(glGetUniformLocation(miniCubo1.Program, "model"), 1, GL_FALSE, glm::value_ptr(spotCube1.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniView3, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj3, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		spotCube1.Draw();

		glUniformMatrix4fv(glGetUniformLocation(miniCubo1.Program, "model"), 1, GL_FALSE, glm::value_ptr(spotCube2.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniView3, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj3, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		spotCube2.Draw();

		glUniformMatrix4fv(glGetUniformLocation(miniCubo1.Program, "model"), 1, GL_FALSE, glm::value_ptr(pointCube2.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniView3, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj3, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		pointCube2.Draw();

		glUniformMatrix4fv(glGetUniformLocation(miniCubo1.Program, "model"), 1, GL_FALSE, glm::value_ptr(pointCube1.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniView3, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj3, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		pointCube1.Draw();

		

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement(GLFWwindow *window) {
	camera.DoMoviment(window, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	/*
	else if (key == GLFW_KEY_F && action == GLFW_PRESS && WIDEFRAME == false) {
	WIDEFRAME = true;

	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS && WIDEFRAME == true) {
	WIDEFRAME = false;
	}
	/*else if ((key == GLFW_KEY_1 && action == GLFW_PRESS)) {
	switcher -= 0.1;
	}
	else if ((key == GLFW_KEY_2 && action == GLFW_PRESS)) {
	switcher += 0.1;
	}
	else if ((key == GLFW_KEY_RIGHT && action == GLFW_PRESS)) {
	rotateRight = true;
	}
	else if ((key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)) {
	rotateRight = false;
	}
	else if ((key == GLFW_KEY_LEFT && action == GLFW_PRESS)) {
	rotateLeft = true;
	}
	else if ((key == GLFW_KEY_LEFT && action == GLFW_RELEASE)) {
	rotateLeft = false;
	}

	else if ((key == GLFW_KEY_UP && action == GLFW_PRESS)) {
	rotateUp = true;
	}
	else if ((key == GLFW_KEY_UP && action == GLFW_RELEASE)) {
	rotateUp = false;
	}
	else if ((key == GLFW_KEY_DOWN && action == GLFW_PRESS)) {
	rotateDown = true;
	}
	else if ((key == GLFW_KEY_DOWN && action == GLFW_RELEASE)) {
	rotateDown = false;
	}

	else if ((key == GLFW_KEY_1 && action == GLFW_PRESS)) {
	mo1 = true;
	mo2 = false;
	mo3 = false;
	}
	else if ((key == GLFW_KEY_2 && action == GLFW_PRESS)) {
	mo1 = false;
	mo2 = true;
	mo3 = false;
	}
	else if ((key == GLFW_KEY_3 && action == GLFW_PRESS)) {
	mo1 = false;
	mo2 = false;
	mo3 = true;
	}*/

	//mover cubo grande 
	else if (key == GLFW_KEY_I && action == GLFW_PRESS)
		cubPos.y = 0.01f;
	else if (key == GLFW_KEY_I && action == GLFW_RELEASE)
		cubPos.y = 0.f;

	else if (key == GLFW_KEY_K && action == GLFW_PRESS)
		cubPos.y = -0.01f;
	else if (key == GLFW_KEY_K && action == GLFW_RELEASE)
		cubPos.y = 0.f;

	else if (key == GLFW_KEY_J && action == GLFW_PRESS)
		cubPos.x = 0.01f;
	else if (key == GLFW_KEY_J && action == GLFW_RELEASE)
		cubPos.x = 0.f;

	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
		cubPos.x = -0.01f;
	else if (key == GLFW_KEY_L && action == GLFW_RELEASE)
		cubPos.x = 0.f;

	else if (key == GLFW_KEY_U && action == GLFW_PRESS)
		cubPos.z = 0.01f;
	else if (key == GLFW_KEY_U && action == GLFW_RELEASE)
		cubPos.z = 0.f;

	else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		cubPos.z = -0.01f;
	else if (key == GLFW_KEY_O && action == GLFW_RELEASE)
		cubPos.z = 0.f;

	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		cubRot.y = 0.15f;
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		cubRot.y = 0.f;

	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		cubRot.y = -0.15f;
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		cubRot.y = 0.f;

	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		cubRot.x = -0.15f;
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		cubRot.x = 0.f;

	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		cubRot.x = 0.15f;
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		cubRot.x = 0.f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.MouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.MouseScroll(window, xoffset, yoffset);
}

#pragma endregion