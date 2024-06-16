#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
// M3 - Texturas
layout (location = 2) in vec2 texture_coordinates;

out vec4 vertexColor;
// M3 - Texturas
out vec2 textureCoordinates;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(position, 1.0f);
	vertexColor = vec4(color,1.0);

	// M3 - Texturas
	textureCoordinates = vec2(texture_coordinates.x, texture_coordinates.y);
}
