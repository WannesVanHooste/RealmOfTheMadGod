#include "stdafx.h"
#include "HUD.h"


HUD::HUD(Point2f pos)
	:m_Pos{pos}
	,m_pUITexture{ std::make_unique<Texture>("Resources/UI.png") }
	,m_pHpTexture{ std::make_unique<Texture>("HP" , "Resources/FogSans.otf", 20,Color4f{ 1.0f,1.0f,1.0,1.0f }) }
	,m_pMannaTexture{ std::make_unique<Texture>("MP" , "Resources/FogSans.otf", 20,Color4f{ 1.0f,1.0f,1.0,1.0f } )}
	,m_pMinimap{ std::make_unique<MiniMap>() }
{
}

void HUD::Draw(std::shared_ptr<Avatar> avatar) const
{
	m_pUITexture->Draw({ m_Pos.x,m_Pos.y }, { 0,0,m_pUITexture->GetWidth(),m_pUITexture->GetHeight() });
	DrawXp(avatar);
	DrawHp(avatar);
	DrawManna(avatar);
}


void HUD::DrawXp(std::shared_ptr<Avatar> avatar) const
{
	Texture xpTexture{ "Lvl " + std::to_string(avatar->GetLvl()), "Resources/FogSans.otf", 20,{ 1.0f,1.0f,1.0,1.0f } };
	utils::SetColor({ 0.35f,0.5f,0.15f,1.0f });
	utils::FillRect({ 617, 305, 267 * (avatar->GetXp().x / avatar->GetXp().y ),20 });
	xpTexture.Draw({ 620,302 }, { 0,0, xpTexture.GetWidth(), xpTexture.GetHeight() });
}

void HUD::DrawHp(std::shared_ptr<Avatar> avatar) const
{
	if (avatar->GetHp().x > 0)
	{
		utils::SetColor({ 0.88f,0.2f,0.2f,1.0f });
		utils::FillRect({ 617, 276, 267 * (avatar->GetHp().x / avatar->GetHp().y),20 });
	}
	m_pHpTexture->Draw({ 620,273 }, { 0,0,m_pHpTexture->GetWidth(), m_pHpTexture->GetHeight() });
}

void HUD::DrawManna(std::shared_ptr<Avatar> avatar) const
{
	if (avatar->GetManna().x > 0)
	{
		utils::SetColor({ 0.38f,0.52f,0.88f,1.0f });
		utils::FillRect({ 617, 247, 267 * (avatar->GetManna().x / avatar->GetManna().y),20 });
	}
	m_pMannaTexture->Draw({ 620,244 }, { 0,0,m_pMannaTexture->GetWidth(), m_pMannaTexture->GetHeight() });
}

void HUD::DrawMiniMap(std::shared_ptr<Avatar> avatar, std::deque<std::shared_ptr<Enemy>> pEnemies, std::shared_ptr<Texture> texture) const
{
	m_pMinimap->Draw(avatar, pEnemies, texture);
}
