#version 450

layout(binding = 0) uniform CameraUBO {
	mat4 view;
	mat4 projection;

} camera;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour;

layout(location = 0) out vec4 fragColour;

void main(){
	gl_Position = camera.projection * camera.view * vec4(inPosition, 1.0);
	fragColour = inColour;
}