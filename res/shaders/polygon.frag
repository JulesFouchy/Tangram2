#version 430 core

uniform vec2 u_vertices[12];
const int N = 5;

in vec2 vTexCoords;

float intersectionOfHorizRayWith(vec2 nuv, vec2 p1, vec2 p2){
    return (nuv - p1).y / (p2 - p1).y;
}

float evenOdd(vec2 nuv){
    int intersectionsCount = 0;
    for (int i = 0; i < N; ++i){
        vec2 p1 = u_vertices[i];
        vec2 p2 = u_vertices[(i+1)%N];
        float inter = intersectionOfHorizRayWith(nuv, p1 , p2);
        vec2 interPt = p1 + inter * (p2 - p1);
        if (0.0 <= inter && inter <= 1.0 && interPt.x > nuv.x)
            intersectionsCount++;
    }
    return intersectionsCount % 2 == 1 ? 1.0 : 0.0;
}

float nonZeroWinding(vec2 nuv){
    int windingCount = 0;
    for (int i = 0; i < N; ++i){
        vec2 p1 = u_vertices[i];
        vec2 p2 = u_vertices[(i+1)%N];
        float inter = intersectionOfHorizRayWith(nuv, p1 , p2);
        vec2 interPt = p1 + inter * (p2 - p1);
        if (0.0 <= inter && inter <= 1.0 && interPt.x > nuv.x){
            vec2 P1 = nuv - p1;
            vec2 P2 = p2 - p1;
            windingCount += P1.x*P2.y - P1.y*P2.x > 0.0  ? 1 : -1;
        }
    }
    return windingCount != 0 ? 1.0 : 0.0;
}

void main() {
    vec2 nuv = vTexCoords*2.0 - 1.0;
    
    float alpha = evenOdd(nuv);
    vec3 color = vec3(0.8, 0.2, 0.4);
    gl_FragColor = vec4(color, alpha);
}