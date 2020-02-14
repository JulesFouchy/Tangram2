#version 430 core

in vec2 vTexCoords;

uniform sampler2D uSrc;
uniform sampler2D uDst;

void main() {
	vec4 srcCmp = texture(uSrc, vTexCoords);
	vec3 srcCol = srcCmp.rgb;
	float srcAlpha = srcCmp.a;
	vec4 dstCmp = texture(uDst, vTexCoords);
	vec3 dstCol = dstCmp.rgb;
	float dstAlpha = dstCmp.a;
	
	float alpha = srcAlpha + dstAlpha - srcAlpha*dstAlpha;
	float blendf = srcAlpha / alpha;
	vec3 color = blendf * srcCol + (1.-blendf) * dstCol;

	gl_FragColor = vec4(color, alpha);
}