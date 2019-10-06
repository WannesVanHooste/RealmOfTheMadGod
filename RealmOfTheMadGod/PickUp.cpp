#include "stdafx.h"
#include "PickUp.h"


PickUp::PickUp(int index)
	:m_PickUpType{PickUpType(index)}
	,m_Shape{}
	,m_IsActive{false}
	,m_PickUpItem{}
	,m_pLootTile{ std::make_shared<Texture>("Resources/lootspace.png") }
{
	if (index == 1)
	{
		int nrEquipment{ rand() % 4 };
		m_PickUpItem = PickUpItem(nrEquipment);
		m_pPickUpTexture = std::make_shared<Texture>( "Resources/equipment" + std::to_string(nrEquipment) + ".png" );
	}
	else if (index == 2)
	{
		int nrPotion{ rand() % 2 };
		m_PickUpItem = PickUpItem(4 + nrPotion);
		m_pPickUpTexture = std::make_shared<Texture>( "Resources/potion" + std::to_string(nrPotion) + ".png" );
	}
}

PickUp::PickUp(int itemIndex, int i)
	:m_PickUpType{ PickUpType(itemIndex) }
	, m_Shape{}
	,m_IsActive{false}
	, m_PickUpItem{}
{
	if (itemIndex < 4)
	{
		m_pPickUpTexture = std::make_shared<Texture>("Resources/equipment" + std::to_string(itemIndex) + ".png");
	}
	else if (itemIndex >= 4)
	{
		m_pPickUpTexture = std::make_shared<Texture>("Resources/potion" + std::to_string(itemIndex - 4) + ".png");
	}
}

void PickUp::Update(float elapsedSec, const Point2f& pos)
{
	if (m_IsActive)
	{
		m_Shape = { pos.x - m_pPickUpTexture->GetWidth() / 2, pos.y - m_pPickUpTexture->GetHeight() / 2 ,m_pPickUpTexture->GetWidth(), m_pPickUpTexture->GetHeight() };
	}
}

void PickUp::DrawTile() const
{
	//draws the tile where the pickup is presented upon
	if (m_IsActive)
	{
		m_pLootTile->Draw({ 533,0 }, { 0,0,m_pLootTile->GetWidth(), m_pLootTile->GetHeight() });
	}
}

void PickUp::Draw(const Point2f& pos) const
{
	if (m_IsActive)
	{
		m_pPickUpTexture->Draw({ pos.x - m_pPickUpTexture->GetWidth() / 2, pos.y - m_pPickUpTexture->GetHeight() / 2 }, { 0,0,m_pPickUpTexture->GetWidth(), m_pPickUpTexture->GetHeight() });
	}
}

bool PickUp::GetActive()
{
	return m_IsActive;
}

int PickUp::GetItem()
{
	return m_PickUpItem;
}

Rectf PickUp::GetShape()
{
	return m_Shape;
}

void PickUp::Activate()
{
	m_IsActive = true;
}

void PickUp::Deactivate()
{
	m_IsActive = false;
}

std::shared_ptr<Texture> PickUp::GetTexture()
{
	return m_pPickUpTexture;
}

