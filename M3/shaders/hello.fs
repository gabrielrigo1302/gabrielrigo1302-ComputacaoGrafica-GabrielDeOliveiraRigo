#version 450 core
in vec3 vertexColor;
in vec2 textureCoordinates;

out vec4 color;
// M3 - pixels da textura
uniform sampler2D texture_buffer;

void main()
{
    // M3 - Texturas
    color = texture(texture_buffer, textureCoordinates);
}