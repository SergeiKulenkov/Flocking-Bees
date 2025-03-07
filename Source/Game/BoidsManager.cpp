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

	glm::vec2 position;
	glm::vec2 direction;
	glm::vec2 size;
	uint16_t id = 0;

	// boids
	{
		for (Boid& boid : m_Boids)
		{
			size = boid.GetSize();
			position = glm::vec2(Random::RandomInRange(size.x, m_BoidsBoundaries.x - size.x),
								Random::RandomInRange(size.y, m_BoidsBoundaries.y - size.y));
			direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

			boid.Init(position, direction, id);
			m_FlockingData[id].id = id;
			id++;
		}
	}

	// predators
	{
		id = 0;
		for (Predator& predator : m_Predators)
		{
			size = predator.GetSize();
			position = glm::vec2(Random::RandomInRange(size.x, m_BoidsBoundaries.x - size.x),
								Random::RandomInRange(size.y, m_BoidsBoundaries.y - size.y));
			direction = glm::vec2(Random::RandomInRange(-1.f, 1.f), Random::RandomInRange(-1.f, 1.f));

			predator.Init(position, direction, id);
			m_PredatorsData[id].id = id;
			id++;
		}
	}
}

void BoidsManager::Update(float deltaTime)
{
	const auto start = std::chrono::high_resolution_clock::now();

	for (uint8_t index = 0; index < m_PredatorsData.size(); index++)
	{
		m_PredatorsData[index].position = m_Predators[index].GetPosition();
		m_PredatorsData[index].velocity = m_Predators[index].GetVelocity();
	}

	for (Predator& predator : m_Predators)
	{
		SeparatePredators(predator);
		CheckBoundaries(predator);
		predator.Update(deltaTime);
	}

	for (uint16_t index = 0; index < m_FlockingData.size(); index++)
	{
		m_FlockingData[index].position = m_Boids[index].GetPosition();
		m_FlockingData[index].velocity = m_Boids[index].GetVelocity();
	}

	for (Boid& boid : m_Boids)
	{
		Flock(boid);
		AvoidPredators(boid);
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
		for (Predator& predator : m_Predators)
		{
			predator.Draw(*m_DrawList, m_DrawDebugInfo);
		}
	}

	ImGui::SetNextWindowPos(ImVec2(m_BoidsBoundaries.x, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, m_BoidsBoundaries.y));
	ImGui::Begin(windowTitle.data(), NULL, m_WindowFlags);

	ImGui::Checkbox(drawDebugInfoText.data(), &m_DrawDebugInfo);
	ImGui::Text(numberOfBoidsText.data(), numberOfBoids);
	ImGui::Text(numberOfPredatorsText.data(), numberOfPredators);
	ImGui::Text(updateTimeText.data(), m_Duration.count() * 1000.f);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text(perceptionRadiusText.data(), Boid::perceptionRadius);
	ImGui::Text(separationRadiusText.data(), Boid::separationRadius);
	ImGui::Text(predatorAvoidanceRadiusText.data(), Boid::predatorAvoidanceRadius);

	ImGui::Text(alignmentWeightText.data(), Boid::allignmentWeight);
	ImGui::Text(cohesionWeightText.data(), Boid::cohesionWeight);
	ImGui::Text(separationWeightText.data(), Boid::separationWeight);

	ImGui::Text(minBoidSpeedText.data(), Boid::minSpeed);
	ImGui::Text(maxBoidSpeedText.data(), Boid::maxSpeed);
	ImGui::End();
}

void BoidsManager::CheckBoundaries(MovingObject& object)
{
	const glm::vec2 position = object.GetPosition();
	glm::vec2 avoidanceDirection = glm::vec2(0, 0);

	if (position.x < edgeMargin)
	{
		avoidanceDirection.x = 1;
	}
	else if (position.x > m_BoidsBoundaries.x - edgeMargin)
	{
		avoidanceDirection.x = -1;
	}

	if (position.y < edgeMargin)
	{
		avoidanceDirection.y = 1;
	}
	else if (position.y > m_BoidsBoundaries.y - edgeMargin)
	{
		avoidanceDirection.y = -1;
	}

	object.AvoidBoundaries(avoidanceDirection);
}

void BoidsManager::Flock(Boid& current)
{
	const float perceptionRadiusSquared = (Boid::perceptionRadius + current.GetRadius()) * (Boid::perceptionRadius + current.GetRadius());
	const float separationRadiusSquared = (Boid::separationRadius + current.GetRadius()) * (Boid::separationRadius + current.GetRadius());
	const glm::vec2 currentPosition = current.GetPosition();
	const uint16_t currentId = current.GetId();

	uint16_t numberOfNeighbours = 0;
	glm::vec2 alignment = glm::vec2(0, 0);
	glm::vec2 cohesion = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);
	glm::vec2 positionDifference = glm::vec2(0, 0);

	for (const MovingObjectData& neighbour : m_FlockingData)
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

void BoidsManager::AvoidPredators(Boid& current)
{
	const float avoidanceRadiusSquared = (Boid::predatorAvoidanceRadius + current.GetRadius()) * (Boid::predatorAvoidanceRadius + current.GetRadius());
	const glm::vec2 currentPosition = current.GetPosition();
	uint16_t numberOfPredators = 0;
	glm::vec2 positionDifference = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);

	for (const Predator& predator : m_Predators)
	{
		positionDifference = currentPosition - predator.GetPosition();
		float distance = glm::dot(positionDifference, positionDifference);

		if (distance < avoidanceRadiusSquared)
		{
			separation += positionDifference / distance;
			numberOfPredators++;
		}
	}

	if (numberOfPredators > 0)
	{
		separation *= numberOfPredators;
		separation = glm::normalize(separation);
		current.UpdateVelocity(separation * Boid::predatorAvoidanceSpeed);
	}
}

void BoidsManager::SeparatePredators(Predator& current)
{
	const float separationRadiusSquared = (Predator::separationRadius + current.GetRadius()) * (Predator::separationRadius + current.GetRadius());
	const glm::vec2 currentPosition = current.GetPosition();
	const uint16_t currentId = current.GetId();

	uint8_t numberOfNeighbours = 0;
	glm::vec2 positionDifference = glm::vec2(0, 0);
	glm::vec2 separation = glm::vec2(0, 0);

	for (const MovingObjectData& neighbour : m_PredatorsData)
	{
		if (currentId != neighbour.id)
		{
			positionDifference = currentPosition - neighbour.position;
			float distance = glm::dot(positionDifference, positionDifference);

			if (distance < separationRadiusSquared)
			{
				separation += positionDifference / distance;
				numberOfNeighbours++;
			}
		}
	}

	if (numberOfNeighbours)
	{
		separation /= numberOfNeighbours;
		separation = glm::normalize(separation) * Predator::maxSpeed;
		separation -= current.GetVelocity();
		current.UpdateAcceleration(separation * Predator::separationWeight);
	}
}
