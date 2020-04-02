#version 430 core

struct Parameters {
    float darkPow; // min 0 max 10
    float sphereDist; // default 8 min 0 max 15
};

in vec2 vTexCoords;

uniform Parameters u;

float nsin(float x){
  return sin(x) * 0.5 + 0.5;  
}

float smin( float a, float b, float k )
{
    float res = exp2( -k*a ) + exp2( -k*b );
    return -log2( res )/k;
}

vec3 pattern2D(vec2 uv){
    
    //vec2 gid = floor(uv);
    //bool b = floor(mod(uv.x, 2.)) + floor(mod(uv.y, 2.)) < 1.;
    //if (b)
    //    return vec3(255, 180, 31) / 255.;
    //else
    //    return vec3(17, 87, 217) / 255.;
    
    //float puls = 6.28;
    //float t = pow(abs( nsin(uv.x * puls) * nsin(uv.y * puls))+0.001, 0.25);
    //t = smoothstep(0.0, 1.0, t);
    //return ( (1.-t) * vec3(255, 180, 31) + (t) * vec3(17, 87, 217) )/255.;
    
    vec2 gv = fract(uv + 0.5) - 0.5;
    float d = length(gv) - 0.4;
    float m = 0.03;
    float t = smoothstep(-m, m, d) 
        	* smoothstep(1.5*m, -1.5*m, abs(gv.x)-0.49)
        	* smoothstep(1.5*m, -1.5*m, abs(gv.y)-0.49);
    //ivec2 gid = ivec2(floor(uv+0.5));
    ivec2 gid = ivec2(floor(uv));
    if ((gid.x + gid.y) % 2 == 0){
    	return ( (t) * vec3(255, 180, 31) + (1.-t) * vec3(17, 87, 217) )/255.;
    }
    else
        return vec3(t);
}

float sdf(vec3 pos){
    // Plane
    float dPlane = abs(pos.z);
    // Sphere
    float dSphere = length(pos - vec3(0., 0., -u.sphereDist)) - 15.;
    //
    return smin(dPlane, dSphere, 0.5);
}

float distort(float x){
  //float dCar = 3.;
  //return pow(dCar / x, 12.) - pow(dCar / x, 8.);
  return exp(-pow(x,2.)*0.04) + 0.7;
}

vec3 normal(vec3 p) {
    float EPSILON = 0.001;
    return normalize(vec3(
        sdf(vec3(p.x + EPSILON, p.y, p.z)) - sdf(vec3(p.x - EPSILON, p.y, p.z)),
        sdf(vec3(p.x, p.y + EPSILON, p.z)) - sdf(vec3(p.x, p.y - EPSILON, p.z)),
        sdf(vec3(p.x, p.y, p.z + EPSILON)) - sdf(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

void main() {
    vec2 uv = vTexCoords*0.9;
    vec3 rayOrg = vec3(0., 0., 6. + 6.5);
    float focalLength = 0.5;
    vec3 pixelPos = vec3(uv, rayOrg.z - focalLength);
    vec3 rayDir = normalize(pixelPos - rayOrg);
    
    vec3 pos = rayOrg;
    for (int i = 0; i < 600; ++i){
      float d = sdf(pos);
      pos += d * rayDir;
    }
    vec2 uv3D = pos.xy;

    vec3 nor = normal(pos);
    float dark = nor.z;
    dark = pow(dark, u.darkPow);
    
    //uv *= 15.;
    //float r = length(uv);
    //vec2 uv3D = uv / vec2(distort(r), distort(r));
    
    vec3 col = dark * pattern2D(uv3D);
    gl_FragColor = vec4(col, 1.0);
}