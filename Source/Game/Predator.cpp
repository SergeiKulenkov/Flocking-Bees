#include "Predator.h"

////////////////////

Predator::Predator() : MovingObject(predatorImagePath, minSpeed, maxSpeed, boundariesAvoidanceSpeed)
{
}

void Predator::Draw(ImDrawList& drawList, bool drawDebugInfo)
{
	MovingObject::Draw(drawList, drawDebugInfo);
	if (drawDebugInfo)
	{
		drawList.AddCircle(ImVec2(m_CenterPosition.x, m_CenterPosition.y), separationRadius + GetRadius(), predatorRadiusColour);
	}
}
