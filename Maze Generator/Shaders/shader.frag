#version 330

in vec3 o_color; 

out vec4 FragColor;

void main()
{
	FragColor = vec4(o_color, 1.0f);
}