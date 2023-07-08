#version 330

layout (location = 0) out vec4 vFragPos;
uniform sampler2D uFramebufferSampler;

void main() {
    vFragPos = vec4(1.0f);
}