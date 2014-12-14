class PhysicsComponent
{
private:
	std::atomic<unsigned int> frontIndex;
	Math::vec2 position[2];
public:
	void update(float timeDelta)
	{
		unsigned int backIndex = (frontIndex.load() + 1) % 2;
		position[backIndex] = ...
		frontIndex.store(backIndex);
	}

	Math::vec2 getPosition()
	{
		return position[frontIndex.load()];
	}
};