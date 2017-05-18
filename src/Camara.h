#pragma once
#include <GLFW\glfw3.h>
#include "../soil/src/SOIL.h"
#include "glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include <vector>
#include <iostream>

class Camara
{
public:
	Camara(glm::vec3, glm::vec3, GLfloat, GLfloat);
	void DoMoviment(GLFWwindow *window, GLfloat);
	void MouseMove(GLFWwindow *window, double xpos, double ypos);
	void MouseScroll(GLFWwindow *window, double xScroll, double yScroll);
	glm::mat4 LookAt();
	GLfloat GetFOV();
	bool moveForward, moveBack, moveLeft, moveRight = false;

	glm::vec3 camPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

private:
	glm::vec3 camUp;
	glm::vec3 camRight;
	glm::vec3 camDir;
	GLfloat camSpeed;
	GLfloat lastFrame;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat sens;
	GLboolean firstMouse;
	GLfloat PITCH, YAW;
	GLfloat fov;

};

Camara::Camara(glm::vec3 position, glm::vec3 direction, GLfloat sensitivity, GLfloat FOV)
{
	camPos = position;
	camDir = direction;
	sens = sensitivity;
	fov = FOV;
	camDir = glm::normalize(camPos);

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f); // world
	camUp = glm::cross(camDir, camRight);

	//camRight = glm::normalize(glm::cross(camDir, camUp));
}

void Camara::DoMoviment(GLFWwindow *window, GLfloat delta)
{
	camSpeed = 3 *delta * 3;
//	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	int FORWARD = glfwGetKey(window, GLFW_KEY_W);
	int BACKWARD = glfwGetKey(window, GLFW_KEY_S);
	int RIGHT = glfwGetKey(window, GLFW_KEY_D);
	int LEFT = glfwGetKey(window, GLFW_KEY_A);


	//Press
	if (FORWARD == GLFW_PRESS) {
		moveForward = true;
	}
	if (BACKWARD == GLFW_PRESS) {
		moveBack = true;
	}
	if (RIGHT == GLFW_PRESS) {
		moveRight = true;
	}
	if (LEFT == GLFW_PRESS) {
		moveLeft = true;
	}
	//Release
	if (FORWARD == GLFW_RELEASE) {
		moveForward = false;
	}
	if (BACKWARD == GLFW_RELEASE) {
		moveBack = false;
	}
	if (RIGHT == GLFW_RELEASE) {
		moveRight = false;
	}
	if (LEFT == GLFW_RELEASE) {
		moveLeft = false;
	}

	if(moveForward) camPos += cameraFront*camSpeed;
	if(moveBack) camPos -= cameraFront*camSpeed;
	if(moveRight)camPos += normalize(cross(cameraFront, camUp))*camSpeed;
	if (moveLeft)camPos -= normalize(cross(cameraFront, camUp))*camSpeed;

}

void Camara::MouseMove(GLFWwindow *window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	//Sensibilidad
	
	xoffset *= sens;
	yoffset *= sens;


	//Angulo de la camara en relación a la posición del ratón
	PITCH += yoffset;
	YAW += xoffset;


	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.0f;
	YAW = glm::mod(YAW + xoffset, 360.0f);
	glm::vec3 front;

	front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	front.y = sin(glm::radians(PITCH));
	front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	cameraFront = glm::normalize(front);
	camRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
	camUp = glm::normalize(glm::cross(camRight, cameraFront));
}

void Camara::MouseScroll(GLFWwindow *window, double xScroll, double yScroll) {

	if (fov >= 1.0f && fov <= 80.0f)
		fov -= yScroll;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 80.0f)
		fov = 80.0f;
}

glm::mat4 Camara::LookAt() {
	return glm::lookAt(camPos, camPos + cameraFront, camUp);
}

GLfloat Camara::GetFOV() {
	return fov;
}