#version 430 core

in vec2 vTexCoords;

void main(){
    float d = length(vTexCoords - vec2(0.5));
    float alpha = smoothstep(0.5,0.29, d);
    gl_FragColor = vec4(vTexCoords, 0.0, 1.0);
}