#include "BoidsManager.h"
#include "Game.h"

////////////////////

BoidsManager::BoidsManager()
{	
}

BoidsManager::~BoidsManager()
{
	m_DrawList = nullptr;
}

void BoidsManager::Init(const ImVec2& windowSize)
{
	m_BoidsBoundaries = windowSize;
	m_BoidsBoundaries.x -= windowWidth;
	m_WindowFlags |= ImGuiWindowFlags_NoMove;
	m_WindowFlags |= ImGuiWindowFlags_NoResize;
	m_WindowFlags |= ImGuiWindowFlags_NoCollapse;

	uint16_t id = 0;
	glm::vec2 position;
	glm::vec2 direction;
	const glm::vec2 boidSize = m_Boids[0].GetSize(); // they all have the same size

	for (Boid& boid : m_Boids)
	{
		position = glm::vec2(Random::RandomInRange(boidSize.x, m_BoidsBoundaries.x - boidSize.x),
							Random::RandomInRange(boidSize.y, m_BoidsBoundaries.y - boidSize.y));
		direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

		boid.Init(position, direction, id);
		m_FlockingData[id].id = id;
		id++;
	}
}

void BoidsManager::Update(float deltaTime)
{
	const auto start = std::chrono::high_resolution_clock::now();

	for (uint16_t index = 0; index < m_FlockingData.size(); index++)
	{
		m_FlockingData[index].position = m_Boids[index].GetPosition();
		m_FlockingData[index].velocity = m_Boids[index].GetVelocity();
	}

	for (Boid& boid : m_Boids)
	{
		Flock(boid);
		CheckBoundaries(boid);
		boid.Update(deltaTime);
	}

	m_Duration = std::chrono::high_resolution_clock::now() - start;
}

void BoidsManager::Draw()
{
	m_DrawList = ImGui::GetBackgroundDrawList();
	if (m_DrawList != nullptr)
	{
		for (Boid& boid : m_Boids)
		{
			boid.Draw(*m_DrawList, m_DrawDebugInfo);
		}
	}

	ImGui::SetNextWindowPos(ImVec2(m_BoidsBoundaries.x, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, m_BoidsBoundaries.y));
	ImGui::Begin(windowTitle.data(), NULL, m_WindowFlags);

	ImGui::Checkbox(drawDebugInfoText.data(), &m_DrawDebugInfo);
	ImGui::Text(numberOfBoidsText.data(), numberOfBoids);
	ImGui::Text(updateTimeText.data(), m_Duration.count() * 1000.f);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text(perceptionRadiusText.data(), Boid::perceptionRadius);
	ImGui::Text(separationRadiusText.data(), Boid::separationRadius);

	ImGui::Text(alignmentWeightText.data(), Boid::allignmentWeight);
	ImGui::Text(cohesionWeightText.data(), Boid::cohesionWeight);
	ImGui::Text(separationWeightText.data(), Boid::separationWeight);

	ImGui::Text(minBoidSpeedText.data(), Boid::minSpeed);
	ImGui::Text(maxBoidSpeedText.data(), Boid::maxSpeed);
	ImGui::End();
}

void BoidsManager::CheckBoundaries(Boid& boid)
{
	const glm::vec2 position = boid.GetPosition();

	if (position.x < edgeMargin)
	{
		boid.UpdateVelocity(glm::vec2(Random::RandomInRange(Boid::minEdgeAvoidanceSpeed, Boid::maxEdgeAvoidanceSpeed), 0));
	}
	else if (position.x > m_BoidsBoundaries.x - edgeMargin)
	{
		boid.UpdateVelocity(glm::vec2(-Random::RandomInRange(Boid::minEdgeAvoidanceSpeed, Boid::maxEdgeAvoidanceSpeed), 0));
	}

	if (position.y < edgeMargin)
	{
		boid.UpdateVelocity(glm::vec2(0, Random::RandomInRange(Boid::minEdgeAvoidanceSpeed, Boid::maxEdgeAvoidanceSpeed)));
	}
	else if (position.y > m_BoidsBoundaries.y - edgeMargin)
	{
		boid.UpdateVelocity(glm::vec2(0, -Random::RandomInRange(Boid::minEdgeAvoidanceSpeed, Boid::maxEdgeAvoidanceSpeed)));
	}
}

void BoidsManager::Flock(Boid& current)
{
	static constexpr float perceptionRadiusSquared = Boid::perceptionRadius * Boid::perceptionRadius;
	static constexpr float separationRadiusSquared = Boid::separationRadius * Boid::separationRadius;
	const glm::vec2 currentPosition = current.GetPosition();
	const uint16_t currentId = current.GetId();

	uint16_t numberOfNeighbours = 0;
	glm::vec2 alignment = glm::vec2(0, 0);
	glm::vec2 cohesion = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);
	glm::vec2 positionDifference = glm::vec2(0, 0);

	for (const FlockingData& neighbour : m_FlockingData)
	{
		if (neighbour.id != currentId)
		{
			positionDifference = currentPosition - neighbour.position;
			float distance = glm::dot(positionDifference, positionDifference);

			if (distance < perceptionRadiusSquared)
			{
				alignment += neighbour.velocity;
				cohesion += neighbour.position;

				if (distance < separationRadiusSquared)
				{
					separation += positionDifference / distance;
				}
				numberOfNeighbours++;
			}
		}
	}

	if (numberOfNeighbours > 0)
	{
		const glm::vec2 currentVelocity = current.GetVelocity();
		alignment /= numberOfNeighbours;
		alignment = glm::normalize(alignment) * Boid::maxSpeed;
		alignment -= currentVelocity;
		current.UpdateAcceleration(alignment * Boid::allignmentWeight);

		cohesion /= numberOfNeighbours;
		cohesion -= currentPosition;
		cohesion = glm::normalize(cohesion) * Boid::maxSpeed;
		cohesion -= currentVelocity;
		current.UpdateAcceleration(cohesion * Boid::cohesionWeight);

		if (separation != glm::vec2(0, 0))
		{
			separation /= numberOfNeighbours;
			separation = glm::normalize(separation) * Boid::maxSpeed;
			separation -= currentVelocity;
			current.UpdateAcceleration(separation * Boid::separationWeight);
		}
	}
}
