#pragma once
#include "MovingObject.h"

////////////////////

class Predator : public MovingObject
{
public:
	Predator();

	static constexpr float separationRadius = 125.f;
	static constexpr float separationWeight = 1.2f;
	static constexpr float minSpeed = 67.5f;
	static constexpr float maxSpeed = 112.25f;
	static constexpr float boundariesAvoidanceSpeed = 5.f;

private:
	static constexpr std::string_view predatorImagePath = "predator.png";
};