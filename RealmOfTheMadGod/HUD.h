#pragma once
#include "MiniMap.h"

class HUD
{
public:
	HUD(Point2f pos);

	void Draw(std::shared_ptr<Avatar> avatar) const;
	void DrawMiniMap(std::shared_ptr<Avatar> avatar, std::deque<std::shared_ptr<Enemy>> pEnemies, std::shared_ptr<Texture> texture) const;
private:
	std::unique_ptr<Texture> m_pUITexture;
	Point2f m_Pos;
	void DrawXp(std::shared_ptr<Avatar> avatar) const;
	void DrawHp(std::shared_ptr<Avatar> avatar) const;
	void DrawManna(std::shared_ptr<Avatar> avatar) const;
	std::unique_ptr<Texture> m_pHpTexture;
	std::unique_ptr<Texture> m_pMannaTexture;
	std::unique_ptr<MiniMap> m_pMinimap;
};

