#pragma once

enum class Layer {
	None,
	Obstacle,
};

namespace PhysicsLayer {
	static bool CollisionTable[2][2] = {
		true, true,
		true, false,
	};
}