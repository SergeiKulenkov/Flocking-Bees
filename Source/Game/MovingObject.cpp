#include "MovingObject.h"

////////////////////

MovingObject::MovingObject(std::string_view path, float minSpeed, float maxSpeed, float boundariesAvoidanceSpeed)
				: m_MinSpeed(minSpeed), m_MaxSpeed(maxSpeed), m_BoundariesAvoidanceSpeed(boundariesAvoidanceSpeed)
{
	m_Image = std::make_shared<Image>(path);
	m_Size = glm::vec2((float)m_Image->GetWidth(), (float)m_Image->GetHeight());
	m_HalfSize = glm::vec2(m_Size.x / 2.f, m_Size.y / 2.f);
}

MovingObject::~MovingObject()
{
	m_Image = nullptr;
}

void MovingObject::Init(const glm::vec2& position, const glm::vec2& direction, const uint16_t id)
{
	m_CenterPosition = position;
	m_Direction = direction;
	m_Velocity = m_Direction * m_MinSpeed;
	m_Id = id;

	UpdateImageQuad();
	RotateImageQuad();
}

void MovingObject::UpdateImageQuad()
{
	m_ImageQuadPositions[0] = ImVec2(m_CenterPosition.x - m_HalfSize.x, m_CenterPosition.y - m_HalfSize.y);
	m_ImageQuadPositions[1] = ImVec2(m_CenterPosition.x + m_HalfSize.x, m_CenterPosition.y - m_HalfSize.y);
	m_ImageQuadPositions[2] = ImVec2(m_CenterPosition.x + m_HalfSize.x, m_CenterPosition.y + m_HalfSize.y);
	m_ImageQuadPositions[3] = ImVec2(m_CenterPosition.x - m_HalfSize.x, m_CenterPosition.y + m_HalfSize.y);
}

void MovingObject::RotateImageQuad()
{
	const float length = glm::length(m_Direction);
	const float cos = m_Direction.x / length;
	const float sin = m_Direction.y / length;

	for (ImVec2& position : m_ImageQuadPositions)
	{
		const ImVec2 oldPosition = position;
		position.x = m_CenterPosition.x + cos * (oldPosition.x - m_CenterPosition.x) - sin * (oldPosition.y - m_CenterPosition.y);
		position.y = m_CenterPosition.y + sin * (oldPosition.x - m_CenterPosition.x) + cos * (oldPosition.y - m_CenterPosition.y);
	}
}

void MovingObject::Update(float deltaTime)
{
	m_Velocity += m_Acceleration * deltaTime;
	float speed = glm::length(m_Velocity);
	m_Direction = m_Velocity / speed;
	speed = glm::clamp(speed, m_MinSpeed, m_MaxSpeed);
	m_Velocity = m_Direction * speed;
	m_Acceleration = glm::vec2(0, 0);

	m_CenterPosition += m_Velocity * deltaTime;
	UpdateImageQuad();
	RotateImageQuad();
}

void MovingObject::Draw(ImDrawList& drawList, bool drawDebugInfo)
{
	drawList.AddImageQuad((ImTextureID)m_Image->GetDescriptorSet(), m_ImageQuadPositions[0], m_ImageQuadPositions[1], m_ImageQuadPositions[2], m_ImageQuadPositions[3],
		m_ImageQuadUVs[0], m_ImageQuadUVs[1], m_ImageQuadUVs[2], m_ImageQuadUVs[3]);

	if (drawDebugInfo)
	{
		drawList.AddLine(ImVec2(m_CenterPosition.x, m_CenterPosition.y), ImVec2(m_CenterPosition.x + m_Velocity.x, m_CenterPosition.y + m_Velocity.y), defaultColour);
	}
}