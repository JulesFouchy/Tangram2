#version 430 core

in vec2 vTexCoords;

void main() {
    float d = length(vTexCoords - vec2(0.5));
    float alpha = smoothstep(0.5, 0.48, d);
    vec3 color = vec3(smoothstep(0.33, 0.35, d));
    gl_FragColor = vec4(color, alpha);
}