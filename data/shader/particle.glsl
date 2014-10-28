#ifdef VERTEX

uniform vec2 position;

void main()
{
	mat4 m = generateProjection();

	vec4 pos = m  * vec4(vertexPosition + position, 1.0, 1.0);
	gl_Position = pos;
}
#endif

#ifdef FRAGMENT
uniform float remainingTime;
out vec4 color;

void main()
{
	float a = 1.0;

	if(remainingTime <= 2.0)
		a = (remainingTime / 2.0);
	color = vec4(1.0, 1.0, 1.0, a);
}
#endif