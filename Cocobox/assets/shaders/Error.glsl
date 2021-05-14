//Error shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

layout (location = 0) out flat int o_ID;

layout(std140, binding = 0) uniform Scene
{
	mat4 ViewProjectionMatrix;
} u_Scene;

layout(std140, binding = 1) uniform Model
{
	mat4 ModelMatrix;
	float ID;
} u_Model;

void main()
{
	o_ID = int(u_Model.ID);
	gl_Position = u_Scene.ViewProjectionMatrix * u_Model.ModelMatrix * vec4(a_Pos, 1.0);
	//gl_Position = u_Scene.ViewProjectionMatrix * vec4(a_Pos, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in flat int i_ID;

layout(location = 0) out vec4 a_Color;
layout(location = 1) out int a_ID;

void main()
{
	a_Color = vec4(1.0, 0.0, 1.0, 1.0);
	a_ID = i_ID;
}