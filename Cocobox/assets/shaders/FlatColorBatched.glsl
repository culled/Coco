//Batched flat color shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_ID;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in float a_Tex;

layout (location = 0) out vec4 o_Color;
layout (location = 1) out flat int o_ID;
layout (location = 2) out vec2 o_TexCoord;
layout (location = 3) out flat int o_Tex;

layout(std140, binding = 0) uniform Transform
{
	mat4 ViewProjectionMatrix;
	mat4 ModelMatrix;
} u_Transform;

void main()
{
	o_Color = a_Color;
	o_ID = int(a_ID);
	o_TexCoord = a_TexCoord;
	o_Tex = int(a_Tex);

	gl_Position = u_Transform.ViewProjectionMatrix * vec4(a_Pos, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) in vec4 i_Color;
layout (location = 1) in flat int i_ID;
layout (location = 2) in vec2 i_TexCoord;
layout (location = 3) in flat int i_Tex;

layout(location = 0) out vec4 a_Color;
layout(location = 1) out int a_ID;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	a_Color = i_Color * texture(u_Textures[i_Tex], i_TexCoord);
	a_ID = i_ID;
}