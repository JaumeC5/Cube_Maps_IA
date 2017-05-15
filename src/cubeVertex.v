
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

in vec2 texcoord;
out vec2 TexCoord;
out vec3 vertexColor;

void main(){

vertexColor = color;
TexCoord = texcoord;
gl_Position = proj * view * model * vec4(position, 1.0);


}
