#include "Boid.h"

////////////////////

Boid::Boid() : MovingObject(beeImagePath, minSpeed, maxSpeed, boundariesAvoidanceSpeed)
{	
}

void Boid::Draw(ImDrawList& drawList, bool drawDebugInfo)
{
	MovingObject::Draw(drawList, drawDebugInfo);
	if (drawDebugInfo)
	{
		drawList.AddCircle(ImVec2(m_CenterPosition.x, m_CenterPosition.y), perceptionRadius + GetRadius(), defaultColour);
		drawList.AddCircle(ImVec2(m_CenterPosition.x, m_CenterPosition.y), separationRadius + GetRadius(), separationColour);
		drawList.AddCircle(ImVec2(m_CenterPosition.x, m_CenterPosition.y), predatorAvoidanceRadius + GetRadius(), predatorRadiusColour);
	}
}
