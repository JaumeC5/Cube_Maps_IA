#version 330 core

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float alternador;

in vec3 color;
in vec2 TexCoord;
out vec4 outColor;

void main(){

outColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), alternador);

}