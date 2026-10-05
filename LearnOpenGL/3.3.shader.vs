#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 myColor;
uniform float xOffset = 0.0f;

void main()
{
	gl_Position = vec4(aPos + vec3(xOffset, 0.0f, 0.0f), 1.0);
	myColor = aColor;
}