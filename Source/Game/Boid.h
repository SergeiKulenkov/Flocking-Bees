#pragma once
#include "MovingObject.h"

////////////////////

class Boid : public MovingObject
{
public:
	Boid();

	virtual void Draw(ImDrawList& drawList, bool drawDebugInfo = false) override;

	static constexpr float perceptionRadius = 50.f;
	static constexpr float separationRadius = 35.f;
	static constexpr float allignmentWeight = 0.8f;
	static constexpr float cohesionWeight = 0.8f;
	static constexpr float separationWeight = 1.3f;

	static constexpr float predatorAvoidanceRadius = 100.f;
	static constexpr float predatorAvoidanceSpeed = 22.0f;

	static constexpr float minSpeed = 97.5f;
	static constexpr float maxSpeed = 187.75f;
	static constexpr float boundariesAvoidanceSpeed = 8.f;

private:
	static constexpr ImU32 separationColour = 4284148540; // ImVec4(240 / 255.f, 90 / 255.f, 65 / 255.f, 1.f);
	static constexpr ImU32 predatorRadiusColour = 4282473200; // ImVec4(60 / 255.f, 235 / 255.f, 90 / 255.f, 1.f);
	static constexpr std::string_view beeImagePath = "bee.png";
};

