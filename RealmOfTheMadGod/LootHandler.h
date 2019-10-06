#pragma once
#include "LootBag.h"

class LootHandler
{
public:
	LootHandler();
	void Update(float elapsedSec, std::deque<std::shared_ptr<Enemy>> enemies, std::shared_ptr<Avatar> avatar, Inventory& inventory);
	void DrawBags() const;
	void DrawItems(std::shared_ptr<Avatar> avatar) const;
	void ChangeVolume(int value);
private:
	std::deque<std::shared_ptr<LootBag>> m_pLootBags;
	void CheckEnemies(std::deque<std::shared_ptr<Enemy>> enemies);
	std::unique_ptr<SoundEffect> m_pBagSound;
	int m_Volume;
};

