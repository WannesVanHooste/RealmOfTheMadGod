#pragma once
#include "utils.h"
#include "SVGParser.h"

class Avatar;

class RayCast
{
public:
	RayCast();
	void Update(float elapsedSec, const Rectf& shape, Vector2f& horizontalVelocity, Vector2f& verticalVelocity, Vector2f originalSpeed);
private:
	std::vector<std::vector<Point2f>> m_Vertices;
	SVGParser m_LevelBoundaries;
};

