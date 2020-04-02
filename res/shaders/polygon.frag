#version 430 core

struct Parameters {
    float SmoothMin; // default 80 min 5 max 150
    bool useEvenOdd;
    vec3 fill; // default 0.3, 0.6, 0.9
    vec3 stroke; // default 0.8, 0.2, 0.4
    vec2[u.list.size] list; // size 6
};

uniform Parameters u;

//uniform vec2 u_vertices[12];
const int N = 5;

in vec2 vTexCoords;

float intersectionOfHorizRayWith(vec2 nuv, vec2 p1, vec2 p2){
    return (nuv - p1).y / (p2 - p1).y;
}
//? #define u.list.size 1
float evenOdd(vec2 nuv){
    int N = u.list.size;
    int intersectionsCount = 0;
    for (int i = 0; i < N; ++i){
        vec2 p1 = u.list[i];
        vec2 p2 = u.list[(i+1)%N];
        float inter = intersectionOfHorizRayWith(nuv, p1 , p2);
        vec2 interPt = p1 + inter * (p2 - p1);
        if (0.0 <= inter && inter <= 1.0 && interPt.x > nuv.x)
            intersectionsCount++;
    }
    return intersectionsCount % 2 == 1 ? 1.0 : 0.0;
}

float nonZeroWinding(vec2 nuv){
    int N = u.list.size;
    int windingCount = 0;
    for (int i = 0; i < N; ++i){
        vec2 p1 = u.list[i];
        vec2 p2 = u.list[(i+1)%N];
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

float distToSegment(vec2 nuv, vec2 p1, vec2 p2){
    float h = min(1.0, max(0.0, dot(nuv - p1, p2 - p1) / dot(p2 - p1, p2 - p1)));
    return length( nuv - p1 - (p2 - p1) * h);
}
float smin( float a, float b, float k )
{
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}

float strokeDistanceField(vec2 nuv){
    int N = u.list.size;
    float dist = 10000000000000.0;
    for (int i = 0; i < N; ++i){
        vec2 p1 = u.list[i];
        vec2 p2 = u.list[(i+1)%N];
        float d = distToSegment(nuv, p1, p2);
        dist = smin(dist, d, u.SmoothMin);
        //dist = min(dist, d);
    }
    return dist - 0.005;
}

float strokeThroughDistanceField(vec2 nuv){
    return smoothstep(0.007, 0.0, strokeDistanceField(nuv));
}

float SDF(vec2 nuv){
    return strokeDistanceField(nuv) * ( evenOdd(nuv) < 0.5 ? 1.0 : -1.0 );
}

void main() {
    vec2 nuv = vTexCoords;
    
    //float t = SDF(nuv)*0.5 + 0.5;
    //gl_FragColor = vec4(vec3(sin(t*60.0)*0.5+0.5), 1.0);
    //gl_FragColor = vec4(vec3(smoothstep(0.001, -0.001, SDF(nuv))), 1.0);
    
    float alphaStroke = strokeThroughDistanceField(nuv);
    float alphaFill = u.useEvenOdd ? evenOdd(nuv) : nonZeroWinding(nuv);
    vec3 colorStroke = u.stroke;
    vec3 colorFill = u.fill;
    vec3 color = alphaStroke * colorStroke + (1.-alphaStroke) * colorFill;
    if( alphaStroke >= alphaFill)
        color = colorStroke;
    gl_FragColor = vec4(color, alphaStroke + alphaFill);
}