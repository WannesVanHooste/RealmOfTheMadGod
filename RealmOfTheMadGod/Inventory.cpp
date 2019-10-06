#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
	:m_Health{0}
	,m_Manna{0}
	,m_MaxHealth{6}
	,m_MaxManna{6}
	,m_pHealthTexture{ std::make_shared<Texture>("Resources/potion0.png") }
	,m_pMannaTexture{ std::make_shared<Texture>("Resources/potion1.png") }
	,m_pPotionSound{std::make_unique<SoundEffect>("Resources/Audio/Use_potion.ogg")}
	,m_pPickUpSound{std::make_unique<SoundEffect>("Resources/Audio/Pick_up.ogg")}
	,m_Volume{10}
{
	m_pPotionSound->SetVolume(10);
	m_pPickUpSound->SetVolume(10);
}

void Inventory::Draw() const
{
	//draws the items in the inventory and the potions and potion count
	Texture m_HealthCount{ std::to_string(m_Health) + " /  " + std::to_string(m_MaxHealth), "Resources/FogSans.otf", 18,Color4f{ 1.0f,1.0f,1.0,1.0f }};
	Texture m_MannaCount{ std::to_string(m_Manna) + " / " + std::to_string(m_MaxManna), "Resources/FogSans.otf", 18,Color4f{ 1.0f,1.0f,1.0,1.0f } };
	for (int i{ 0 }; i != m_pPickUps.size(); ++i)
	{
		if (i < 4)
		{
			m_pPickUps[i]->Draw({ 652 + float(i * 66), 120 });
		}
		else if (i > 3)
		{
			m_pPickUps[i]->Draw({ 652 + float((i - 4) * 66), 68 });
		}
	}
	m_pHealthTexture->Draw({ 630,8, m_pHealthTexture->GetWidth() - 5, m_pHealthTexture->GetHeight() - 5 }, { 0,0,m_pHealthTexture->GetWidth(), m_pHealthTexture->GetHeight() });
	m_HealthCount.Draw({ 680, 14 });
	m_pMannaTexture->Draw({ 760, 8, m_pMannaTexture->GetWidth() - 5, m_pMannaTexture->GetHeight() - 5 }, { 0,0,m_pMannaTexture->GetWidth(), m_pMannaTexture->GetHeight() });
	m_MannaCount.Draw({ 810, 14 });
}
void Inventory::Update(float elapsedSec, std::shared_ptr<Avatar> avatar)
{
	ConsumePotion(avatar);
	for (int i{}; i != m_pPickUps.size(); ++i)
	{
		m_pPickUps[i]->Activate();
		if (i < 4)
		{
			m_pPickUps[i]->Update(elapsedSec, { 652 + float(i * 66), 120 });
		}
		else if (i > 3)
		{
			m_pPickUps[i]->Update(elapsedSec, { 652 + float((i - 4) * 66), 68 });
		}
	}

	m_AccuSet += elapsedSec;
	if (m_AccuSet > 1.0f)
	{
		m_PotionDelay = true;
		m_AccuSet = 0;
	}
}

void Inventory::AddEquipment(std::shared_ptr<PickUp> pickup)
{
	//adds the pickup on the ground to the inventory
	for (int i{ 0 }; i < m_IsSize; ++i)
	{
		if (m_InventorySlots[i] == false)
		{
			m_pPickUpSound->Play(0);
			m_pPickUps.push_back(std::make_shared<PickUp>(pickup->GetItem(), 0));
			m_InventorySlots[i] = true;
			return;
		}
	}
}

void Inventory::AddPotion(std::shared_ptr<PickUp> pickup)
{
	if (pickup->GetItem() == 4)
	{
		if (m_Health < m_MaxHealth)
		{
			m_pPickUpSound->Play(0);
			++m_Health;
		}
	}
	else if (pickup->GetItem() == 5)
	{
		if (m_Manna < m_MaxManna)
		{
			m_pPickUpSound->Play(0);
			++m_Manna;
		}
	}
}

void Inventory::ConsumePotion(std::shared_ptr<Avatar> avatar)
{
	//consumes potions and gives avatar xp and mp
	const Uint8 *pKeyStates = SDL_GetKeyboardState(nullptr);
	if (m_PotionDelay == true)
	{
		if (avatar->GetHp().x != avatar->GetHp().y)
		{
			if (pKeyStates[SDL_SCANCODE_R])
			{
				if (m_Health > 0)
				{
					m_pPotionSound->Play(0);
					avatar->ReceiveHp(100);
					--m_Health;
					m_PotionDelay = false;
				}
			}
		}
		if (avatar->GetManna().x != avatar->GetManna().y)
		{
			if (pKeyStates[SDL_SCANCODE_F])
			{
				if (m_PotionDelay == true)
				{
					if (m_Manna > 0)
					{
						m_pPotionSound->Play(0);
						avatar->ReceiveManna(100);
						--m_Manna;
						m_PotionDelay = false;
					}
				}
			}
		}
	}
}

void Inventory::ChangeVolume(int value)
{
	if (m_Volume + value >= 0 && m_Volume + value <= 50)
	{
		m_Volume += value;
		m_pPotionSound->SetVolume(m_Volume);
		m_pPickUpSound->SetVolume(m_Volume);
	}
}