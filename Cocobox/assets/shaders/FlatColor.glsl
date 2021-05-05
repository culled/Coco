//Flat color shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Pos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 a_Color;

uniform vec4 u_Color;

void main()
{
	a_Color = u_Color;
}