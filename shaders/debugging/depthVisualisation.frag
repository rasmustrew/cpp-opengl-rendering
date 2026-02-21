#version 330 core
out vec4 FragColor;

uniform float near;
uniform float far;

float LinearizeDepth(float depth, float near, float far) {
    float ndc = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - ndc * (far - near));
}

void main() {
    float depth = LinearizeDepth(gl_FragCoord.z, near, far) / far;
    FragColor = vec4(vec3(depth), 1.0);
}