#pragma once
#include <array>
#include <chrono>

#include "../App/DrawableObject.h"

#include "Boid.h"
#include "Predator.h"

////////////////////

struct MovingObjectData
{
	uint16_t id = 0;
	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 velocity = glm::vec2(0, 0);
};

////////////////////

class BoidsManager : public IDrawableObject
{
public:
	BoidsManager();
	~BoidsManager();

	void Init(const ImVec2& windowSize);

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	void CheckBoundaries(MovingObject& object);
	void Flock(Boid& current);
	void AvoidPredators(Boid& current);
	void SeparatePredators(Predator& current);

	static constexpr uint16_t numberOfBoids = 100;
	static constexpr uint8_t numberOfPredators = 3;
	static constexpr float windowWidth = 200.f;
	static constexpr float edgeMargin = 60.f;

	static constexpr std::string_view windowTitle = "Boids Manager Stats";
	static constexpr std::string_view drawDebugInfoText = "Draw Debug Info";
	static constexpr std::string_view numberOfBoidsText = "Number of Bodis = %d";
	static constexpr std::string_view numberOfPredatorsText = "Number of Predators = %d";
	static constexpr std::string_view updateTimeText = "Update Time = %.2f";

	static constexpr std::string_view perceptionRadiusText = "Perception Radius = %.2f";
	static constexpr std::string_view separationRadiusText = "Separation Radius = %.2f";
	static constexpr std::string_view predatorAvoidanceRadiusText = "Predator Avoidance = %.2f";
	static constexpr std::string_view alignmentWeightText = "Alignment Weight = %.2f";
	static constexpr std::string_view cohesionWeightText = "Cohesion Weight = %.2f";
	static constexpr std::string_view separationWeightText = "Separation Weight = %.2f";
	static constexpr std::string_view minBoidSpeedText = "Min Speed = %.2f";
	static constexpr std::string_view maxBoidSpeedText = "Max Speed = %.2f";

	ImDrawList* m_DrawList = nullptr;

	std::array<Boid, numberOfBoids> m_Boids;
	std::array<MovingObjectData, numberOfBoids> m_FlockingData;
	// probably better to move predators to a separate PredatorManager
	std::array<Predator, numberOfPredators> m_Predators;
	std::array<MovingObjectData, numberOfPredators> m_PredatorsData;	

	ImVec2 m_BoidsBoundaries = ImVec2(0, 0);
	ImGuiWindowFlags m_WindowFlags = 0;

	std::chrono::duration<float> m_Duration = std::chrono::milliseconds(0);
	bool m_DrawDebugInfo = false;
};