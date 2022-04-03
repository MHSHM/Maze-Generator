#version 330 core 

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_translation;
layout (location = 2) in vec3 in_color;

uniform mat4 u_orthoproj;

out vec3 o_color;

void main()
{
	gl_Position = u_orthoproj * vec4(in_position + in_translation, 1.0f);
	o_color = in_color; 
}