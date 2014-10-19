#ifdef VERTEX

uniform float width;
uniform float height;

void main()
{
	float right = width;
	float left = 0.0;
	float bottom = height;
	float top = 0.0;

	mat4 m = generateProjection();

	vec4 pos = m * vec4(vertexPosition, 1.0, 1.0);
	gl_Position = pos;
}
#endif

#ifdef FRAGMENT
in vec3 Color;
out vec4 outColor;

uniform float factor;

uniform mat4 mat;
void main()
{
	vec3 fooColor = mix(vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), factor);
	outColor = vec4(fooColor, 1.0);
}
#endif