class GameObject
{
public:
	// Advances the game objects simulation by a given time step
	virtual void update(float timeDelta) = 0;

	virtual void draw() = 0;

	virtual const CollisionComponent* getCollisionComponent() const = 0;
	virtual const PhysicsComponent* getPhysicsComponent() const = 0;

	bool collidesWith(const GameObject& other);
	bool collidesWith(const GameObject* other);
};