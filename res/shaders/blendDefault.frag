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
	
	vec3 color = srcAlpha * srcCol + (1.-srcAlpha) * dstCol;
	float alpha = srcAlpha + dstAlpha - srcAlpha*dstAlpha;

	gl_FragColor = vec4(color, alpha);
}