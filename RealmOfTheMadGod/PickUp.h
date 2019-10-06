#pragma once
#include "Texture.h"
#include <memory>
#include <iostream>

class PickUp
{
public:
	enum PickUpType {
		equipment = 1,
		potion = 2
	};
	enum PickUpItem {
		staff = 0,
		robe = 1,
		spell = 2,
		ring = 3,
		health = 4,
		manna = 5
	};

	PickUp(int index);
	PickUp(int itemIndex, int i);
	void Draw(const Point2f& pos) const;
	void DrawTile() const;
	void Update(float elapsedSec, const Point2f& pos);
	Rectf GetShape();
	void Activate();
	std::shared_ptr<Texture> GetTexture();
	void Deactivate();
	int GetItem();
	bool GetActive();
private:
	std::shared_ptr<Texture> m_pPickUpTexture;
	std::shared_ptr<Texture> m_pLootTile;
	PickUpType m_PickUpType;
	PickUpItem m_PickUpItem;
	Rectf m_Shape;
	bool m_IsActive;
};

