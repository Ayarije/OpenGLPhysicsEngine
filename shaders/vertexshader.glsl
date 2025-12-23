#version 330 core

// Vertex attributes
layout(location = 0) in vec3 vertexPos;    // Vertex position in model space
layout(location = 1) in vec2 texCoord;     // Texture coordinates
layout(location = 2) in vec3 normal;       // Vertex normal in model space

// Uniforms (provided by the application)
uniform mat4 ProjMat;       // Projection matrix
uniform mat4 ViewMat;       // View (camera) matrix
uniform mat4 ModelMat;      // Model (object) matrix
uniform vec3 LightPosition; // Light position in world space

// Outputs to fragment shader
out vec2 UV;                         // Texture coordinates passed through
out vec3 Position_worldspace;        // Vertex position in world space
out vec3 EyeDirection_cameraspace;   // Vector from vertex to camera in camera space
out vec3 LightDirection_cameraspace; // Vector from vertex to light in camera space
out vec3 Normal_cameraspace;         // Normal vector transformed to camera space

void main() {
    // Clip-space position
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(vertexPos, 1.0);

    // Pass texture coordinates unchanged
    UV = texCoord;

    // Transform vertex position to world space: Model * position_model
    vec4 pos_world = ModelMat * vec4(vertexPos, 1.0);
    Position_worldspace = pos_world.xyz;

    // Transform vertex position to camera (view) space: View * Model * position_model
    vec3 vertexPosition_cameraspace = (ViewMat * pos_world).xyz;

    // Eye direction in camera space: from vertex toward camera (camera is at origin in camera space)
    EyeDirection_cameraspace = normalize(-vertexPosition_cameraspace);

    // Light position transformed to camera space
    vec3 lightPosition_cameraspace = (ViewMat * vec4(LightPosition, 1.0)).xyz;

    // Direction from vertex toward light, in camera space
    LightDirection_cameraspace = normalize(lightPosition_cameraspace - vertexPosition_cameraspace);

    // Transform normal to camera space. Use w = 0 since normals are directions, then normalize.
    Normal_cameraspace = normalize((ViewMat * ModelMat * vec4(normal, 0.0)).xyz);
}