#ifdef VERTEX

uniform vec2 position;
uniform float size;

void main()
{
	float right = 800;
	float left = 0.0;
	float bottom = 600;
	float top = 0.0;

	mat4 m = generateProjection();

	mat2 scale = mat2(
		size, 0.0,
		0.0, size
	);

	vec4 pos = m  * vec4((vertexPosition * scale) + position, 1.0, 1.0);
	gl_Position = pos;
}
#endif

#ifdef FRAGMENT
out vec4 color;

void main()
{
	color = vec4(vec3(1.0, 1.0, 1.0), 1.0);
}
#endif