#version 330 core

// Inputs from vertexshader
in vec2 UV;                         // Texture coordinates passed through
in vec3 Position_worldspace;        // Vertex position in world space
in vec3 EyeDirection_cameraspace;   // Vector from vertex to camera in camera space
in vec3 LightDirection_cameraspace; // Vector from vertex to light in camera space
in vec3 Normal_cameraspace;         // Normal vector transformed to camera space

out vec3 color;

uniform sampler2D myTextureSampler;
uniform vec3 LightPosition; // Light position in world space

void main() {
	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;

	vec3 LightColor = vec3(1.0, 1.0, 1.0);
	float LightPower = 40.0;
	
	float cosTheta = clamp(dot(Normal_cameraspace, LightDirection_cameraspace), 0, 1);

	vec3 Reflect = reflect(-LightDirection_cameraspace, Normal_cameraspace);
	float cosAlpha = clamp(dot(EyeDirection_cameraspace, Reflect), 0, 1);

	vec3 diff = Position_worldspace - LightPosition;
	float dist_sqrd = dot(diff, diff);

	color =
	MaterialAmbientColor +
	MaterialDiffuseColor * LightColor * LightPower * cosTheta / dist_sqrd +
	MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 5) / dist_sqrd;
}