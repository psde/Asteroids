struct State
{
	Math::vec2 position;
	// ...
};

class PhysicsComponent
{
private:
	State position;
	// ...
};

class GameObject
{
private:
	PhysicsComponent physics;
	State renderState;
	// ...
};