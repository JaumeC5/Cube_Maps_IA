//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include "Camara.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include "Model.h"
#include "Object.h"

using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
GLsizei numBuffers = 3;
float switcher = 0.0;
float angleX = 0.0;
float angleY = 0.0;
bool rotateLeft, rotateRight, rotateUp, rotateDown;
float aspectRatio = WIDTH / HEIGHT;
float timeNow, lastTime, deltaTime;
bool firstMouse = true;
GLfloat lastX = 400, lastY = 300;
bool mo1, mo2, mo3 = false;

glm::vec3 lightColor = glm::vec3(0.6f, 0.0f, 0.5);

Camara cam(glm::vec3(0.0f, 2.0f, 3.f), glm::normalize(glm::vec3(0.f, 0.f, 3.f) - glm::vec3(0.f, 0.f, 0.f)), 0.1, 60);

glm::vec3 cubRot = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 cubScal = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 cubPos = glm::vec3(0.f, 0.f, 0.f);




void mouseController(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroller(GLFWwindow* window, double xoffset, double yoffset);
void Do_Moviment(GLFWwindow *window);

glm::vec3 lightPos(0.f, 2.f, 3.0f); // posición de la luz


int main() {
	//initGLFW
	auto t_start = std::chrono::high_resolution_clock::now();
	mo1 = true;
	if (!glfwInit())
		::exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//create a window
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Trasform.", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);


	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}

	//viewPort
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//set function when callback
	Do_Moviment(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseController);
	glfwSetScrollCallback(window, scroller);


	//fondo
	glClearColor(0.5, 0, 0.5, 1.0);

	//cargamos los shader
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

	//Objects

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

	// load textures, cubo simple
	/*
	int width, height;
	unsigned char *img;

	GLint texAttrib = glGetAttribLocation(textureShader.Program, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(6 * sizeof(float)));

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img = SOIL_load_image("ying.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	SOIL_free_image_data(img);


	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img = SOIL_load_image("dick.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	SOIL_free_image_data(img);
	*/

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


	//ordre dels vertex dels triangles del quadrat
	GLuint indexBufferObject[] = {
		0, 1, 3,
		1, 2, 3
	};

	bigC.Initiate();
	miniCube.Initiate();
	pointCube1.Initiate();
	pointCube2.Initiate();
	spotCube1.Initiate();
	spotCube2.Initiate();
	directional.Initiate();
	bigCube.Initiate();
	// Crear los VBO, VAO y EBO y reservar memoria para el VAO, VBO y EBO
	/*
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);				//vao per pintar el plà i tot lo altre
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);	// vao per pintar el cub

	//delete[] vertexBufferObject;

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferObject), indexBufferObject, GL_STATIC_DRAW);

	//////////////Starts config for the plane//////////////////////
/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	//////////////Finishes config for the plane//////////////////////

	//////////////Starts config for the cube//////////////////////

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	//////////////Finishes config for the cube//////////////////////
	*/

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

	glm::mat4 model;

	GLfloat radio = 8.0f;

	glm::mat4 view;
	glEnable(GL_DEPTH_TEST);

	//Raton

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Inicializar algunos parámetros de los cubos

	bigC.Scale(cubScal);
	miniCube.Move(glm::vec3(0.f, 2.f, 3.0f));
	miniCube.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	spotCube1.Move(lightPositions[0]);
	spotCube1.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	pointCube1.Move(lightPositions[1]);
	pointCube1.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	spotCube2.Move(lightPositions[2]);
	spotCube2.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	pointCube2.Move(lightPositions[3]);
	pointCube2.Scale(glm::vec3(0.1f, 0.1f, 0.1f));


	directional.Move(lightPositions[4]);
	directional.Scale(glm::vec3(0.1f, 0.1f, 0.1f));

	bigCube.Move(glm::vec3(3.f, 1.f, 0.f));
	bigCube.Scale(glm::vec3(5.f, 5.f, 5.f));


	//bucle de dibujado
	while (!glfwWindowShouldClose(window)) {
		lastTime = glfwGetTime();

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borrar buffer de colors i de profunditat

		//Establecer el color de fondo
		glClearColor(0.f, 0.f, 0.f, 1.0);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10, 10, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Gets the time
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();



		glm::mat4 proj = glm::perspective(glm::radians(cam.GetFOV()), aspectRatio, 1.0f, 1000.0f);

		view = cam.LookAt();

		//establecer el shader

		//squareShader.USE(); //descomentar aixo si volem veure es quadrat
		//textureShader.USE();
		//cubeShader.USE();
		//modelShader.USE();

		// passar valors a les variables dels shaders
#if(true)
		glUniform1f(variableShader, abs(sin(glfwGetTime()) / 2));
		glUniform1f(imgSwitcher, switcher);

		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
#endif
		//comprovar si rotar foto o cubo
		if (rotateLeft)
			angleY += 0.2;
		if (rotateRight)
			angleY -= 0.2;

		if (rotateDown)
			angleX -= 0.2;
		if (rotateUp)
			angleX += 0.2;

		// mesclar foto
		if (switcher > 1)
			switcher = 1;

		if (switcher < 0.1)
			switcher = 0;

		//activar textures cubo simple
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(textureShader.Program, "tex1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(textureShader.Program, "tex2"), 1);
		*/

		

		//Shader a canviar per sa iluminació
		multiShader.USE();

		bigC.Move(cubPos);
		bigC.Rotate(cubRot);

		//Escena con cublo de luz
#if(false)
		glUniform3f(glGetUniformLocation(lightShader.Program, "objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(lightShader.Program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightShader.Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(materialShader.Program, "viewPos"), cam.camPos.x, cam.camPos.y, cam.camPos.z);

		glUniformMatrix4fv(uniView2, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj2, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(bigC.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel
#endif
		//Escena amb es material
#if(false)

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
		glUniform3f(glGetUniformLocation(materialShader.Program, "viewPos"), cam.camPos.x, cam.camPos.y, cam.camPos.z);

		//Luz
		glUniform3f(glGetUniformLocation(materialShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(materialShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(materialShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(materialShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);

		//Material
		glUniform3f(glGetUniformLocation(materialShader.Program, "material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(materialShader.Program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(materialShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(materialShader.Program, "material.shininess"), 32.f);



		glUniformMatrix4fv(glGetUniformLocation(materialShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(bigC.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel

		glUniformMatrix4fv(uniView4, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj4, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
#endif
		//Escena amb textura i material
#if(false)
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		glUniform3f(glGetUniformLocation(niceCubem8.Program, "viewPos"), cam.camPos.x, cam.camPos.y, cam.camPos.z);
		glUniform3f(glGetUniformLocation(niceCubem8.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(niceCubem8.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(niceCubem8.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(niceCubem8.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(niceCubem8.Program, "material.shininess"), 32.f);
		glUniform1i(glGetUniformLocation(niceCubem8.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(niceCubem8.Program, "material.specular"), 1);

		glUniformMatrix4fv(uniView5, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj5, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(glGetUniformLocation(niceCubem8.Program, "model"), 1, GL_FALSE, glm::value_ptr(bigC.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel

		//activar texturas cubo de luz
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
#endif
		//Point Light
#if(false)
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		glUniform3f(glGetUniformLocation(pointShader.Program, "viewPos"), cam.camPos.x, cam.camPos.y, cam.camPos.z);
		//Light
		glUniform3f(glGetUniformLocation(pointShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(pointShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(pointShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(pointShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(pointShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(pointShader.Program, "light.linear"), 0.09);
		glUniform1f(glGetUniformLocation(pointShader.Program, "light.quadratic"), 0.032);

		//Material
		glUniform1f(glGetUniformLocation(pointShader.Program, "material.shininess"), 32.f);
		glUniform1i(glGetUniformLocation(pointShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(pointShader.Program, "material.specular"), 1);

		glUniformMatrix4fv(uniView6, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj6, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(glGetUniformLocation(pointShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(bigC.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel

																																   //activar texturas cubo de luz
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
#endif
		//spotLight
#if(false)
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		glUniform3f(glGetUniformLocation(spotShader.Program, "viewPos"), cam.camPos.x, cam.camPos.y, cam.camPos.z);
		//Light
		glUniform1f(lightSpotCutOffLoc, glm::cos(glm::radians(12.5f)));
		glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(17.5f)));
		glUniform3f(lightSpotdirLoc, cam.cameraFront.x, cam.cameraFront.y, cam.cameraFront.z);
		glUniform3f(glGetUniformLocation(spotShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(spotShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(spotShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(spotShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(spotShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(spotShader.Program, "light.linear"), 0.09);
		glUniform1f(glGetUniformLocation(spotShader.Program, "light.quadratic"), 0.032);

		//Material
		glUniform1f(glGetUniformLocation(spotShader.Program, "material.shininess"), 32.f);
		glUniform1i(glGetUniformLocation(spotShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(spotShader.Program, "material.specular"), 1);

		glUniformMatrix4fv(uniView7, 1, GL_FALSE, glm::value_ptr(view)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(uniProj7, 1, GL_FALSE, glm::value_ptr(proj)); // transferir el que val model al uniform on apunta uniModel
		glUniformMatrix4fv(glGetUniformLocation(spotShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(bigC.GetModelMatrix())); // transferir el que val model al uniform on apunta uniModel

																																	//activar texturas cubo de luz
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
#endif
		//multilight
#if(true)
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		glUniform3f(glGetUniformLocation(multiShader.Program, "viewPos"), cam.camPos.x, cam.camPos.y, cam.camPos.z);
		//directional
		glUniform3f(glGetUniformLocation(multiShader.Program, "dirLight.direction"), 0.f, -0.3f, -0.8f);
		glUniform3f(glGetUniformLocation(multiShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
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
#endif
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

		


		//bind antic VAO
		/*glBindVertexArray(VAO); {

			//dibujado models 3d
#if(false)
			if(mo1)
			m1.Draw(modelShader);
			if(mo2)
				m2.Draw(modelShader);
			if(mo3)
				m3.Draw(modelShader);
#endif
			//cubo controlable

			glm::mat4 trans, rot, rot1,  rot2;
			trans = glm::translate(trans, CubesPositionBuffer[0]); // matriu de translació
			rot1 = glm::rotate(rot, glm::radians(angleY), glm::vec3(0.0f, 1.f, 0.0f)); // matriu que controla rotació en Y
			rot2 = glm::rotate(rot, glm::radians(angleX), glm::vec3(1.0f, 0.f, 0.0f)); // matriu qu controla rotació en X
			rot = rot1*rot2; // matriu resultant de roatr en els dos eixos
			model = trans * rot; // model és igual a translladar i rotar
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model)); // transferir el que val model al uniform on apunta uniModel
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
//cubos independents
#if(false)
			for (int i = 1; i < 10; i++) {
				glm::mat4 trans, rot;
				trans = glm::translate(trans, CubesPositionBuffer[i]);
				rot = glm::rotate(rot, (-time)*glm::radians(180.f), glm::vec3(1.0f, 1.f, 0.0f));
				model = trans * rot;
				glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

#endif
		}
		*/
		// posició de sa càmera

		//pintar el VAO
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);

		// cuadrat que s'estira
#if(false)
		if (WIDEFRAME) {

			//pintar amb linies
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glDrawElements(GL_LINE_STRIP, 7, GL_UNSIGNED_INT, (GLvoid*)0);
			glBindVertexArray(0);

		}

		else {
			//pintar amb triangles
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
			glBindVertexArray(0);

		}
#endif
		timeNow = glfwGetTime();
		deltaTime = timeNow - lastTime;
		// Swap the screen buffers. !!!!!!!!Molt important!!!!!
		glfwSwapBuffers(window);
		//comprueba que algun disparador se haya activado (tales como el teclado, raton, etc)
		Do_Moviment(window);
		glfwPollEvents();
	}

	//rentar sa memoria
	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/
	// Terminate GLFW, clearing any resources allocated by GLFW.
	::exit(EXIT_SUCCESS);
}

void Do_Moviment(GLFWwindow *window) {
	cam.DoMoviment(window, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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
		cubPos.y = 0.001f;
	else if (key == GLFW_KEY_I && action == GLFW_RELEASE)
		cubPos.y = 0.f;

	else if (key == GLFW_KEY_K && action == GLFW_PRESS)
		cubPos.y = -0.001f;
	else if (key == GLFW_KEY_K && action == GLFW_RELEASE)
		cubPos.y = 0.f;

	else if (key == GLFW_KEY_J && action == GLFW_PRESS)
		cubPos.x = 0.001f;
	else if (key == GLFW_KEY_J && action == GLFW_RELEASE)
		cubPos.x = 0.f;

	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
		cubPos.x = -0.001f;
	else if (key == GLFW_KEY_L && action == GLFW_RELEASE)
		cubPos.x = 0.f;

	else if (key == GLFW_KEY_U && action == GLFW_PRESS)
		cubPos.z = 0.001f;
	else if (key == GLFW_KEY_U && action == GLFW_RELEASE)
		cubPos.z = 0.f;

	else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		cubPos.z = -0.001f;
	else if (key == GLFW_KEY_O && action == GLFW_RELEASE)
		cubPos.z = 0.f;

	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		cubRot.y = 0.05f;
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		cubRot.y = 0.f;

	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		cubRot.y = -0.05f;
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		cubRot.y = 0.f;

	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		cubRot.x = -0.05f;
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		cubRot.x = 0.f;

	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		cubRot.x = 0.05f;
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		cubRot.x = 0.f;




}

void mouseController(GLFWwindow* window, double xpos, double ypos) { // working!

	cam.MouseMove(window, xpos, ypos);
}


//Zoom. Cambia el FOV
void scroller(GLFWwindow *window, double xoffset, double yoffset) {

	cam.MouseScroll(window, xoffset, yoffset);


}