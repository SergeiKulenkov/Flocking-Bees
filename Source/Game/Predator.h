#pragma once
#include "MovingObject.h"

////////////////////

class Predator : public MovingObject
{
public:
	Predator();

	virtual void Draw(ImDrawList& drawList, bool drawDebugInfo = false) override;

	static constexpr float separationRadius = 125.f;
	static constexpr float separationWeight = 1.5f;
	static constexpr float minSpeed = 67.5f;
	static constexpr float maxSpeed = 112.25f;
	static constexpr float boundariesAvoidanceSpeed = 5.f;

private:
	static constexpr std::string_view predatorImagePath = "predator.png";
	static constexpr ImU32 predatorRadiusColour = 4282473200; // ImVec4(60 / 255.f, 235 / 255.f, 90 / 255.f, 1.f);
};