#ifdef VERTEX
in vec2 position;
in vec3 color;
out vec3 Color;

uniform float width;
uniform float height;

uniform mat4 mat;

void main()
{
	float right = 800.0;
	float left = 0.0;
	float bottom = 600.0;
	float top = 0.0;

	mat4 m = mat4(
		2.0/(right-left), 0.0, 0.0, 0.0,
		0.0, 2.0/(top-bottom), 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		-(right+left)/(right-left), -(top+bottom)/(top-bottom), 0.0, 1.0
	);

	vec4 p = m * vec4(position, 1.0, 1.0);

	Color = color;
	gl_Position = p;
}
#endif

#ifdef FRAGMENT
in vec3 Color;
out vec4 outColor;

uniform float factor;
uniform float width;
uniform float height;

uniform mat4 mat;
void main()
{
	vec3 fooColor = mix(vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), factor);
	outColor = vec4(fooColor, 1.0);
}
#endif