#pragma once
#include <memory>
#include <array>

#include "glm/glm.hpp"
#include "imgui.h"

#include "../App/Image.h"

////////////////////

class IMovableObject
{
public:
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(ImDrawList& drawList, bool drawDebugInfo = false) = 0;
	virtual void Init(const glm::vec2& position, const glm::vec2& direction, const uint16_t id) = 0;

protected:
	virtual void UpdateImageQuad() = 0;
	virtual void RotateImageQuad() = 0;
};

////////////////////

class MovingObject : public IMovableObject
{
public:
	MovingObject(std::string_view path, float minSpeed, float maxSpeed, float boundariesAvoidanceSpeed);

	~MovingObject();

	bool operator==(const MovingObject& other) const
	{
		return m_Id == other.GetId();
	}

	virtual void Update(float deltaTime) override;
	virtual void Draw(ImDrawList& drawList, bool drawDebugInfo = false) override;
	virtual void Init(const glm::vec2& position, const glm::vec2& direction, const uint16_t id) override;

	uint16_t GetId() const { return m_Id; }
	glm::vec2 GetSize() const { return m_Size; }
	glm::vec2 GetPosition() const { return m_CenterPosition; }
	glm::vec2 GetHalfSize() const { return m_HalfSize; }
	glm::vec2 GetVelocity() const { return m_Velocity; }
	float GetRadius() const { return (m_HalfSize.x + m_HalfSize.y) / 2; }

	void UpdateAcceleration(const glm::vec2& acceleration) { m_Acceleration += acceleration; }
	void UpdateVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }
	void AvoidBoundaries(const glm::vec2& direction) { UpdateVelocity(direction * m_BoundariesAvoidanceSpeed); }

protected:
	virtual void UpdateImageQuad() override;
	virtual void RotateImageQuad() override;

	static constexpr ImU32 defaultColour = 4282503900; // ImVec4(220 / 255.f, 210 / 255.f, 65 / 255.f, 1.f);
	static constexpr std::array<ImVec2, 4> m_ImageQuadUVs = { ImVec2(0, 0), ImVec2(1, 0), ImVec2(1, 1), ImVec2(0, 1) };

	uint16_t m_Id = 0;
	glm::vec2 m_CenterPosition = glm::vec2(0, 0);
	glm::vec2 m_Direction = glm::vec2(0, 0);
	glm::vec2 m_Velocity = glm::vec2(0, 0);
	glm::vec2 m_Acceleration = glm::vec2(0, 0);
	float m_MinSpeed = 0.f;
	float m_MaxSpeed = 0.f;
	float m_BoundariesAvoidanceSpeed = 0.f;

	std::shared_ptr<Image> m_Image;
	glm::vec2 m_Size = glm::vec2(0, 0);
	glm::vec2 m_HalfSize = glm::vec2(0, 0);
	std::array<ImVec2, 4> m_ImageQuadPositions = { ImVec2(), ImVec2(), ImVec2(), ImVec2() };
};