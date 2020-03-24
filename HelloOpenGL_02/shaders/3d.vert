#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertex.xyz, 1.0);
    TexCoords = aTexCoords;
}
