#version 430 core

uniform vec2 u_vertices[12];

in vec2 vTexCoords;

float intersectionOfHorizRayWith(vec2 nuv, vec2 p1, vec2 p2){
    return (nuv - p1).y / (p2 - p1).y;
}

void main() {
    vec2 nuv = vTexCoords*2.0 - 1.0;

    int intersectionsCount = 0;
    int N = 3;
    for (int i = 0; i < N; ++i){
        vec2 p1 = u_vertices[i];
        vec2 p2 = u_vertices[(i+1)%N];
        float inter = intersectionOfHorizRayWith(nuv, p1 , p2);
        vec2 interPt = p1 + inter * (p2 - p1);
        if (0.0 <= inter && inter <= 1.0 && interPt.x > nuv.x)
            intersectionsCount++;
    }
    
    float alpha = intersectionsCount % 2 == 1 ? 1.0 : 0.0;
    vec3 color = vec3(0.8, 0.2, 0.4);
    gl_FragColor = vec4(color, alpha);
}