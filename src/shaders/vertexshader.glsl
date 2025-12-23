# Vertex Shader GLSL Code

# This is a basic vertex shader that transforms vertex positions and passes through texture coordinates.
# You can modify it according to your rendering needs.

# Vertex shader input
# layout(location = 0) in vec3 position; // Vertex position
# layout(location = 1) in vec2 texCoord; // Texture coordinates

# Uniforms
# uniform mat4 projection; // Projection matrix
# uniform mat4 view; // View matrix
# uniform mat4 model; // Model matrix

# Output to fragment shader
# out vec2 fragTexCoord; // Pass texture coordinates to fragment shader

# Main function
# void main() {
#     gl_Position = projection * view * model * vec4(position, 1.0); // Transform vertex position
#     fragTexCoord = texCoord; // Pass texture coordinates
# }