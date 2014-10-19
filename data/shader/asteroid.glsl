#ifdef VERTEX
layout(location = 0) in vec2 vertexPosition;

uniform vec2 position;

void main()
{
	float right = 800;
	float left = 0.0;
	float bottom = 600;
	float top = 0.0;

	mat4 m = mat4(
		2.0/(right-left), 0.0, 0.0, 0.0,
		0.0, 2.0/(top-bottom), 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		-(right+left)/(right-left), -(top+bottom)/(top-bottom), 0.0, 1.0
	);

	vec4 pos = m  * vec4(vertexPosition + position, 1.0, 1.0);
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