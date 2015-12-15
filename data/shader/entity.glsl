#ifdef VERTEX
uniform vec2 position;
uniform float size = 1.0f;

void main()
{
	mat4 m = generateProjection();

	vec4 pos = m * vec4((vertexPosition * generateScale(size))+ position, 1.0, 1.0);
	gl_Position = pos;
}
#endif

#ifdef FRAGMENT
out vec4 color;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}
#endif