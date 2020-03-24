#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    FragColor = texture(image, TexCoord)*vec4(spriteColor,1.0);
}