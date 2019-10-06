#pragma once
#include "PickUp.h"
#include "Avatar.h"

class Inventory
{
public:
	Inventory();
	void Draw() const;
	void AddEquipment(std::shared_ptr<PickUp> pickup);
	void AddPotion(std::shared_ptr<PickUp> pickup);
	void Update(float elapsedSec, std::shared_ptr<Avatar> avatar);
	void ConsumePotion(std::shared_ptr<Avatar> avatar);
	void ChangeVolume(int value);
private:
	float m_AccuSet;
	bool m_PotionDelay;
	static const int m_IsSize{ 8 };
	bool m_InventorySlots[m_IsSize];
	std::deque<std::shared_ptr<PickUp>> m_pPickUps;
	int m_Health;
	int m_Manna;
	int m_MaxHealth;
	int m_MaxManna;
	std::shared_ptr<Texture> m_pHealthTexture;
	std::shared_ptr<Texture> m_pMannaTexture;
	std::unique_ptr<SoundEffect> m_pPotionSound;
	std::unique_ptr<SoundEffect> m_pPickUpSound;
	int m_Volume;
};

