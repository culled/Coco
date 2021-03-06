//Flat color shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

layout (location = 0) out vec4 o_Color;
layout (location = 1) out flat int o_ID;

layout(std140, binding = 0) uniform Scene
{
	mat4 ViewProjectionMatrix;
} u_Scene;

layout(std140, binding = 1) uniform Model
{
	mat4 ModelMatrix;
	float ID;
} u_Model;

layout(push_constant) uniform Material
{
	vec4 Color;
} s_Material;

void main()
{
	o_Color = s_Material.Color;
	o_ID = int(u_Model.ID);
	gl_Position = u_Scene.ViewProjectionMatrix * u_Model.ModelMatrix * vec4(a_Pos, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec4 i_Color;
layout (location = 1) in flat int i_ID;

layout(location = 0) out vec4 a_Color;
layout(location = 1) out int a_ID;

void main()
{
	a_Color = i_Color;
	a_ID = i_ID;
}