uniform float time;
uniform mat4 projection;
uniform vec2 windowDimensions;

#ifdef VERTEX
//layout(location = 0) in vec2 vertexPosition;
#endif

mat4 generateProjection()
{
	float right = windowDimensions.x;
	float left = 0.0;
	float bottom = windowDimensions.y;
	float top = 0.0;

	mat4 m = mat4(
		2.0/(right-left), 0.0, 0.0, 0.0,
		0.0, 2.0/(top-bottom), 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		-(right+left)/(right-left), -(top+bottom)/(top-bottom), 0.0, 1.0
	);
	return m;
}
