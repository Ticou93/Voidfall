#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;
uniform mat4 transform;

void main()
{
   	gl_Position = transform * vec4(aPos, 1.0f);
	vertexColor = vec4(0.0f, 0.3f, 0.0f, 1.0f);
	TexCoord    = aTexCoord;	
}