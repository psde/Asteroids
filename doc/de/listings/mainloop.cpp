const unsigned int maxPhysicsStepsPerFrame = 30;
const float physicsTimeStep = 0.001f;
float accumulatedPhysicsTime = 0.f;

while (!window->shouldClose()) {
	unsigned int physicSteps = 0;
	accumulatedPhysicsTime += getTimeDelta();
	while (accumulatedPhysicsTime >= physicsTimeStep)
	{
		// Perform a single update
		game.update(physicsTimeStep);
		accumulatedPhysicsTime -= physicsTimeStep;
		physicSteps++;

		// Do not freeze the game when physics simulation can't
		// keep up but slow down the game instead
		if (physicSteps >= maxPhysicsStepsPerFrame)
			break;
	}

	draw();
	window->finishFrame();
}