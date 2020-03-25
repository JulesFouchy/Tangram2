#version 430 core

in vec2 vTexCoords;

uniform sampler2D uSrc;
uniform sampler2D uDst;

float gamma = 2.0;

void main() {
	vec4 srcCmp = texture(uSrc, vTexCoords);
	vec3 srcCol = srcCmp.rgb;
	float srcAlpha = srcCmp.a;
	vec4 dstCmp = texture(uDst, vTexCoords);
	vec3 dstCol = dstCmp.rgb;
	float dstAlpha = dstCmp.a;
	
	float alpha = srcAlpha + dstAlpha - srcAlpha*dstAlpha;
	float blendf = srcAlpha / alpha;
	vec3 color = pow(blendf * pow(srcCol, vec3(gamma)) + (1.-blendf) * pow(dstCol, vec3(gamma)), vec3(1./gamma)); // Gamma correction ! https://www.youtube.com/watch?v=LKnqECcg6Gw

	gl_FragColor = vec4(color, alpha);
}