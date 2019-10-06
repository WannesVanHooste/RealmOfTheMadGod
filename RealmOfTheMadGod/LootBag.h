#pragma once
#include "Inventory.h"

class LootBag
{
public:
	LootBag(const Point2f& pos);
	void Draw() const;
	void Update(float elapsedSec, std::shared_ptr<Avatar> avatar, Inventory& inventory);
	void DrawItems(std::shared_ptr<Avatar> avatar);
	void PickItemUp(Inventory& inventory);
	bool GetActive();
	std::shared_ptr<PickUp> GetPickUp();
private:
	Rectf m_Shape;
	float m_AccuSec;
	std::shared_ptr<Texture> m_pLootBagTexture;
	Point2f m_Position;
	bool m_Empty;
	bool m_TimeOver;
	std::shared_ptr<PickUp> m_pPickUp;
	bool CheckEmpy();
	void CheckTime(float elapsedSec);
	void CheckActive(std::shared_ptr<Avatar> avatar);
};

