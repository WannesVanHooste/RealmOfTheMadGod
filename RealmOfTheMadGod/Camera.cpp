#include "stdafx.h"
#include "Camera.h"


Camera::Camera(float width, float height)
	:m_Width{ width }
	, m_height{ height }
	,m_Boundaries {0, 0, 6400.0f, 6400.0f}
{
}

Point2f Camera::GetPosition(const Rectf& toTrack) const
{
	Point2f track{ Track(toTrack) };
	Clamp(track);
	return track;
}

void Camera::SetBoundaries(const Rectf& boundaries)
{
	m_Boundaries = boundaries;
}

Point2f Camera::Track(const Rectf& toTrack) const
{
	return{ toTrack.left + toTrack.width / 2 - m_Width / 2, toTrack.bottom + toTrack.height / 2 - m_height / 2};
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	if (bottomLeftPos.x < m_Boundaries.left)
	{
		bottomLeftPos.x = m_Boundaries.left;
	}
	if (bottomLeftPos.y < m_Boundaries.bottom)
	{
		bottomLeftPos.y = m_Boundaries.bottom;
	}
	if (bottomLeftPos.x > m_Boundaries.width - m_Width)
	{
		bottomLeftPos.x = m_Boundaries.width - m_Width;
	}
	if (bottomLeftPos.y > m_Boundaries.height - m_height)
	{
		bottomLeftPos.y = m_Boundaries.height - m_height;
	}
}