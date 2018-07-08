#version 330

out vec4 fragColor;

void main()
{
	float lerp = (gl_FragCoord.y - 100.0f) / 600.0f;
	fragColor = vec4(lerp, 1.0f - lerp, 1.0f - lerp, 1.0f);
}