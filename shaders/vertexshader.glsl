#version 330 core

layout(location = 0) in vec3 vertexPos; // Vertex position
layout(location = 1) in vec2 texCoord; // Texture coordinates
layout(location = 2) in vec3 normal;

uniform mat4 ProjMat; // Projection matrix
uniform mat4 ViewMat; // View matrix
uniform mat4 ModelMat; // Model matrix

out vec2 UV; // Pass texture coordinates to fragment shader

void main() {
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(vertexPos, 1.0); // Transform vertex position
    UV = texCoord; // Pass texture coordinates
}