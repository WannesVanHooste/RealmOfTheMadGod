#pragma once
#include "utils.h"

class Camera
{
public:
	Camera(float width, float height);
	void SetBoundaries(const Rectf& boundaries);
	Point2f GetPosition(const Rectf& toTrack) const;
private:
	float m_Width;
	float m_height;
	Rectf m_Boundaries;
	Point2f Track(const Rectf& toTrack) const;
	void Clamp(Point2f& bottomLeftPos) const;
};

