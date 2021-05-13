//Flat color shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Pos;

layout (location = 0) out vec4 o_Color;
layout (location = 1) out flat int o_ID;

layout(std140, binding = 0) uniform Transform
{
	mat4 ViewProjectionMatrix;
	mat4 ModelMatrix;
} u_Transform;

layout(std140, binding = 1) uniform Material
{
	int ID;
	vec4 Color;
} u_Material;

void main()
{
	o_Color = u_Material.Color;
	o_ID = u_Material.ID;
	gl_Position = u_Transform.ViewProjectionMatrix * u_Transform.ModelMatrix * vec4(a_Pos, 1.0);
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