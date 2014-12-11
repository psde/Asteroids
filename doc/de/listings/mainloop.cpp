// Physics simulation
const float physicsTimeStep = 0.001f;
const int maxPhysicsStepsPerFrame = 30;
float accumulatedPhysicsTime = 0.f;

float startTime = Graphics::getTime();
float endTime = startTime;

while (!window->shouldClose())
{
	float timeDelta = endTime - startTime;
	startTime = Graphics::getTime();
	
	// Perform physics simulation steps
	unsigned int physicSteps = 0;
	accumulatedPhysicsTime += timeDelta;
	while (accumulatedPhysicsTime >= physicsTimeStep)
	{
		game.update(physicsTimeStep);
		accumulatedPhysicsTime -= physicsTimeStep;
		physicSteps++;

		// Do not freeze the game when physics simulation can't
		// keep up but slow down the game instead
		if (physicSteps >= maxPhysicsStepsPerFrame)
			break;
	}

	game.draw();

	window->finishFrame();
	endTime = Graphics::getTime();
}