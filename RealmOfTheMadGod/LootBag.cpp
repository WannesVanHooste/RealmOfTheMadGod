#include "stdafx.h"
#include "LootBag.h"


LootBag::LootBag(const Point2f& pos)
	:m_Position{pos}
	,m_Empty{false}
	,m_TimeOver{false}
	,m_AccuSec{0}
{
	//generates random loot
	int randomNumber{ rand() % 101 };
	if (randomNumber < 90)
	{
		m_pLootBagTexture = std::make_shared<Texture>( "Resources/lootbag1.png" );
		m_pPickUp = std::make_shared<PickUp>(2);
	}
	else
	{
		m_pLootBagTexture = std::make_shared<Texture>( "Resources/lootbag2.png" );
		m_pPickUp = std::make_shared<PickUp>(1);
	}
}

void LootBag::Update(float elapsedSec, std::shared_ptr<Avatar> avatar, Inventory& inventory)
{
	if (m_TimeOver == false)
	{
		if (CheckEmpy() == false)
		{
			CheckActive(avatar);
			m_Shape = { m_Position.x - m_pLootBagTexture->GetWidth() / 2, m_Position.y - m_pLootBagTexture->GetHeight() / 2, m_pLootBagTexture->GetWidth(), m_pLootBagTexture->GetHeight() };
			CheckTime(elapsedSec);
			PickItemUp(inventory);
		}
	}
}

void LootBag::Draw() const
{
	if (m_Empty == false && m_TimeOver == false)
	{
		m_pLootBagTexture->Draw({ m_Position.x - m_pLootBagTexture->GetWidth() / 2, m_Position.y - m_pLootBagTexture->GetHeight() / 2 }, { 0,0,m_pLootBagTexture->GetWidth(), m_pLootBagTexture->GetHeight() });
	}
}

bool LootBag::CheckEmpy()
{
	return m_Empty;
}

void LootBag::CheckTime(float elapsedSec)
{
	//makes the loot disappear after 20 seconds
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= 20.f)
	{
		m_TimeOver = true;
		m_pPickUp->Deactivate();
	}
}

void LootBag::DrawItems(std::shared_ptr<Avatar> avatar)
{
	m_pPickUp->DrawTile();
	m_pPickUp->Draw({ 566.5f, 28 });
}

void LootBag::CheckActive(std::shared_ptr<Avatar> avatar)
{
	//checks when player is on bag
	if (utils::IsOverlapping(avatar->GetShape(), m_Shape))
	{
		m_pPickUp->Activate();
	}
	else {
		m_pPickUp->Deactivate();
	}
}

std::shared_ptr<PickUp> LootBag::GetPickUp()
{
	return m_pPickUp;
}

void LootBag::PickItemUp(Inventory& inventory)
{
	//adding item to inventory
	if (m_pPickUp->GetActive() == true)
	{
		const Uint8 *pKeyStates = SDL_GetKeyboardState(nullptr);
		if (pKeyStates[SDL_SCANCODE_Q])
		{
			if (m_pPickUp->GetItem() < 4)
			{
				inventory.AddEquipment(m_pPickUp);
			}
			else if (m_pPickUp->GetItem() >= 4)
			{
				inventory.AddPotion(m_pPickUp);
			}
			m_Empty = true;
			m_pPickUp->Deactivate();
		}
	}
}

bool LootBag::GetActive()
{
	if (m_Empty == true || m_TimeOver == true)
	{
		return true;
	}
	return false;
}
