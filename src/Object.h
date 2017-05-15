#pragma once
#include <GL\glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

using namespace glm;
	enum FigureType {
		cube = 0
	};
class Object{
	
public:


	Object(vec3 scale, vec3 rotation, vec3 position, FigureType type);
	~Object();

	void Initiate();
	void Draw();
	void Move(vec3 translation);
	void Rotate(vec3 rota);
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix();
	vec3 GetPosition();
	GLuint VBO, VAO, EBO;


private:
	mat4 model, trans, rot, rot1, rot2, skale;
	vec3 position;
	vec3 scale;
	vec3 rotation;	
};

