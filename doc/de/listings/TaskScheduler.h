class PhysicsComponent
{
private:
	Math::vec2 position;
public:
	void update(float timeDelta, PhysicsScheduler scheduler)
	{
		if(isUpdateNeeded() == false)
			return;
		auto func = std::bind(&PhysicsComponent:updateWorker, this);
		scheduler.schedule(func);
	}

	void updateWorker()
	{
		position = ...;
	}
};

class PhysicsScheduler
{
private:
	concurrent_queue<std::function<void(void)> tasks;

public:
	void schedule(std::function<void(void) func)
	{
		tasks.push_back(func);
	}

	void work()
	{
		for(auto &task : tasks)
			std::async(launch::async, task);
	}
};