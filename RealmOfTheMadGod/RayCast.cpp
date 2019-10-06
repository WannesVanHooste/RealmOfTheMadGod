#include "stdafx.h"
#include "RayCast.h"
#include "Avatar.h"

RayCast::RayCast()
	:m_Vertices{}
	, m_LevelBoundaries{}
{
	m_LevelBoundaries.GetVerticesFromSvgFile("Resources/LevelSvg2.svg", m_Vertices);
}

void RayCast::Update(float elapsedSec, const Rectf& shape, Vector2f& horizontalVelocity, Vector2f& verticalVelocity, Vector2f originalSpeed)
{
	//initialze the 9 most important points for the raycast and checks whether there is collision
	utils::HitInfo hitinfo{};
	Point2f mid{ shape.left + (shape.width / 2), shape.bottom + (shape.height / 2) };
	Point2f left{ shape.left, shape.bottom + (shape.height / 2) };
	Point2f right{ shape.left + shape.width, shape.bottom + (shape.height / 2) };
	Point2f up{ shape.left + (shape.width / 2), shape.bottom + shape.height };
	Point2f down{ shape.left + (shape.width / 2), shape.bottom};
	Point2f rightUp{ shape.left + shape.width, shape.bottom + shape.height };
	Point2f rightDown{ shape.left + shape.width, shape.bottom };
	Point2f leftUp{ shape.left, shape.bottom + shape.height };
	Point2f leftDown{ shape.left, shape.bottom };
	if (utils::Raycast(m_Vertices[0],mid, left, hitinfo) == true)
	{
		horizontalVelocity.x *= 0;
	}
	else if (utils::Raycast(m_Vertices[0], mid, right, hitinfo) == true)
	{
		horizontalVelocity.y *= 0;
	}
	else if (utils::Raycast(m_Vertices[0], mid, up, hitinfo) == true)
	{
		verticalVelocity.x *= 0;
	}
	else if (utils::Raycast(m_Vertices[0], mid, down, hitinfo) == true)
	{
		verticalVelocity.y *= 0;
	}
	else if (utils::Raycast(m_Vertices[0], mid, rightUp, hitinfo) == true)
	{
		horizontalVelocity.y *= 0;
		verticalVelocity.x *= 0;
	}
	else if (utils::Raycast(m_Vertices[0], mid, rightDown, hitinfo) == true)
	{
		horizontalVelocity.y *= 0;
		verticalVelocity.y *= 0;
	}
	else if (utils::Raycast(m_Vertices[0], mid, leftUp, hitinfo) == true)
	{
		horizontalVelocity.x *= 0;
		verticalVelocity.x *= 0;
	}
	else if (utils::Raycast(m_Vertices[0], mid, rightUp, hitinfo) == true)
	{
		horizontalVelocity.x *= 0;
		verticalVelocity.y *= 0;
	}
	else {
		horizontalVelocity = originalSpeed;
		verticalVelocity = originalSpeed;
	}
}
