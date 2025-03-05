#pragma once
#include <memory>
#include <array>

#include "glm/glm.hpp"
#include "imgui.h"

#include "../App/Image.h"

////////////////////

class Boid
{
public:
	Boid();

	bool operator==(const Boid& other) const
	{
		return m_Id == other.GetId();
	}

	void Init(const glm::vec2& position, const glm::vec2& direction, const uint16_t id);

	void Update(float deltaTime);

	void Draw(ImDrawList& drawList, bool drawDebugInfo = false);

	uint16_t GetId() const { return m_Id; }
	glm::vec2 GetSize() const { return m_Size; }
	glm::vec2 GetHalfSize() const { return m_HalfSize; }
	glm::vec2 GetPosition() const { return m_CenterPosition; }
	glm::vec2 GetVelocity() const { return m_Velocity; }

	void UpdateAcceleration(const glm::vec2& acceleration) { m_Acceleration += acceleration; }
	void UpdateVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }

	static constexpr float perceptionRadius = 70.f;
	static constexpr float separationRadius = 45.f;
	static constexpr float allignmentWeight = 0.8f;
	static constexpr float cohesionWeight = 0.8f;
	static constexpr float separationWeight = 1.3f;

	static constexpr float minEdgeAvoidanceSpeed = 15.f;
	static constexpr float maxEdgeAvoidanceSpeed = 25.f;
	static constexpr float minSpeed = 97.5f;
	static constexpr float maxSpeed = 187.75f;

private:
	void UpdateImageQuad();
	void RotateImageQuad();

	static constexpr ImU32 defaultColour = 4282503900; // ImVec4(220 / 255.f, 210 / 255.f, 65 / 255.f, 1.f);
	static constexpr ImU32 separationColour = 4282473200; // ImVec4(240 / 255.f, 90 / 255.f, 65 / 255.f, 1.f);
	static constexpr std::string_view beeImagePath = "bee.png";
	static constexpr std::array<ImVec2, 4> m_ImageQuadUVs = { ImVec2(0, 0), ImVec2(1, 0), ImVec2(1, 1), ImVec2(0, 1) };

	uint16_t m_Id = 0;
	glm::vec2 m_CenterPosition = glm::vec2(0, 0);
	glm::vec2 m_Direction = glm::vec2(0, 0);
	glm::vec2 m_Velocity = glm::vec2(0, 0);
	glm::vec2 m_Acceleration = glm::vec2(0, 0);

	std::shared_ptr<Image> m_Image;
	glm::vec2 m_Size = glm::vec2(0, 0);
	glm::vec2 m_HalfSize = glm::vec2(0, 0);
	std::array<ImVec2, 4> m_ImageQuadPositions = { ImVec2(), ImVec2(), ImVec2(), ImVec2() };
};

