#pragma once
#include <array>
#include <chrono>

#include "../App/Object.h"

#include "Boid.h"

////////////////////

struct FlockingData
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
	void CheckBoundaries(Boid& boid);
	void Flock(Boid& current);

	static constexpr uint16_t numberOfBoids = 200;
	static constexpr float windowWidth = 200.f;
	static constexpr float edgeMargin = 30.f;

	static constexpr std::string_view windowTitle = "Boids Manager Stats";
	static constexpr std::string_view drawDebugInfoText = "Draw Debug Info";
	static constexpr std::string_view numberOfBoidsText = "Number of Bodis = %d";
	static constexpr std::string_view updateTimeText = "Update Time = %.2f";

	static constexpr std::string_view perceptionRadiusText = "Perception Radius = %.2f";
	static constexpr std::string_view separationRadiusText = "Separation Radius = %.2f";
	static constexpr std::string_view alignmentWeightText = "Alignment Weight = %.2f";
	static constexpr std::string_view cohesionWeightText = "Cohesion Weight = %.2f";
	static constexpr std::string_view separationWeightText = "Separation Weight = %.2f";
	static constexpr std::string_view minBoidSpeedText = "Min Speed = %.2f";
	static constexpr std::string_view maxBoidSpeedText = "Max Speed = %.2f";

	ImDrawList* m_DrawList = nullptr;

	std::array<Boid, numberOfBoids> m_Boids;
	std::array<FlockingData, numberOfBoids> m_FlockingData;

	ImVec2 m_BoidsBoundaries = ImVec2(0, 0);
	ImGuiWindowFlags m_WindowFlags = 0;

	std::chrono::duration<float> m_Duration = std::chrono::milliseconds(0);
	bool m_DrawDebugInfo = false;
};