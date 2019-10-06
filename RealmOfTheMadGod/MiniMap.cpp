#include "stdafx.h"
#include "MiniMap.h"


MiniMap::MiniMap()
	:m_PlayerRect{745, 475, 5,5}
	,m_pMiniMapCamera{std::make_unique<Camera>(600.f,600.f)}
{
}

void MiniMap::Draw(std::shared_ptr<Avatar> avatar, std::deque<std::shared_ptr<Enemy>> pEnemies, std::shared_ptr<Texture> texture) const
{
	utils::SetColor({ 0.17f,0.29f,0.545f,1.f });
	utils::FillRect({ 650,375,200,200 });

	glPushMatrix();
	glTranslatef(-m_pMiniMapCamera->GetPosition(avatar->GetShape()).x / 8, -m_pMiniMapCamera->GetPosition(avatar->GetShape()).y / 8, 0);
	texture->Draw({ 710,440, texture->GetWidth() * 0.125f, texture->GetHeight() * 0.125f }, { 0,0,texture->GetWidth(),texture->GetHeight() });
	for (std::shared_ptr<Enemy> element : pEnemies)
	{
		if (element->IsDead() == false)
		{
			utils::SetColor({ 1.f,0.f,0.f,1.f });
			utils::FillRect((element->GetPosition().x / 6400 * 800) + 713, (element->GetPosition().y / 6400 * 800) + 435, 5, 5);
		}
	}
	glPopMatrix();

	utils::SetColor({ 0.f,0.f,1.f,1.f });
	utils::FillRect(m_PlayerRect);
}
