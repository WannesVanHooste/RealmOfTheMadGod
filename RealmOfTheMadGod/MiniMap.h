#pragma once
#include "Avatar.h"

class MiniMap
{
public:
	MiniMap();
	void Draw(std::shared_ptr<Avatar> avatar, std::deque<std::shared_ptr<Enemy>> pEnemies, std::shared_ptr<Texture> texture) const;
private:
	Rectf m_PlayerRect;
	std::unique_ptr<Camera> m_pMiniMapCamera;
	Point2f m_MidPoint; 
};

