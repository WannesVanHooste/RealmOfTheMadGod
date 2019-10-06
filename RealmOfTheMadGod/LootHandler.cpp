#include "stdafx.h"
#include "LootHandler.h"


LootHandler::LootHandler()
	:m_pBagSound{std::make_unique<SoundEffect>("Resources/Audio/Loot_appears.ogg")}
	,m_Volume{10}
{
	m_pBagSound->SetVolume(15);
}

void LootHandler::Update(float elapsedSec, std::deque<std::shared_ptr<Enemy>> enemies, std::shared_ptr<Avatar> avatar, Inventory& inventory)
{
	CheckEnemies(enemies);
	for (std::shared_ptr<LootBag> element : m_pLootBags)
	{
		element->Update(elapsedSec, avatar, inventory);
	}
	//if lootbag is deactivated it gets deleted
	for (int i{ 0 }; i != m_pLootBags.size(); ++i)
	{
		if (m_pLootBags.size() > 1)
		{
			if (m_pLootBags[i]->GetActive() == true)
			{
				m_pLootBags[i].swap(m_pLootBags[0]);
				m_pLootBags.pop_front();
				i = 0;
			}
		}
	}
}

void LootHandler::DrawBags() const
{
	for (std::shared_ptr<LootBag> element : m_pLootBags)
	{
		element->Draw();
	}
}

void LootHandler::DrawItems(std::shared_ptr<Avatar> avatar) const
{
	for (std::shared_ptr<LootBag> element : m_pLootBags)
	{
		element->DrawItems(avatar);
	}
}

void LootHandler::CheckEnemies(std::deque<std::shared_ptr<Enemy>> enemies)
{
	//if enemy dies a lootbag appears
	for (std::shared_ptr<Enemy> element : enemies)
	{
		if (element->IsDead() == true)
		{
			m_pBagSound->Play(0);
			m_pLootBags.push_back(std::make_shared<LootBag>(element->GetPosition()));
		}
	}
}

void LootHandler::ChangeVolume(int value)
{
	if (m_Volume + value >= 5 && m_Volume + value <= 55)
	{
		m_Volume += value;
		m_pBagSound->SetVolume(m_Volume);
	}
}
