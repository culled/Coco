//Unlit Texture shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

layout (location = 0) out flat int o_ID;
layout (location = 1) out vec2 o_TexCoords;

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
	o_TexCoords = a_TexCoords;
	o_ID = int(u_Model.ID);
	gl_Position = u_Scene.ViewProjectionMatrix * u_Model.ModelMatrix * vec4(a_Pos, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in flat int i_ID;
layout (location = 1) in vec2 i_TexCoords;

layout(location = 0) out vec4 a_Color;
layout(location = 1) out int a_ID;

layout(push_constant) uniform Material
{
	vec4 Tint;
} s_Material;

layout(binding = 5) uniform sampler2D u_Texture;

void main()
{
	a_Color = texture(u_Texture, i_TexCoords) * s_Material.Tint;
	a_ID = i_ID;
}