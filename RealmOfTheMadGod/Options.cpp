#include "stdafx.h"
#include "Options.h"


Options::Options()
	:m_IsOn(false)
	,m_pOptionsTexture{std::make_unique<Texture>("Resources/Options.png")}
	,m_pOptionsSound{std::make_unique<SoundEffect>("Resources/Audio/Options.ogg")}
	,m_pControlsTexture{std::make_unique<Texture>("Resources/Controls.png")}
	,m_Volume{10}
	,m_Controls{false}
{
	m_pOptionsSound->SetVolume(10);
}

void Options::Draw() const
{
	if (m_IsOn)
	{
		m_pOptionsTexture->Draw({ 0,0 }, { 0,0,m_pOptionsTexture->GetWidth(), m_pOptionsTexture->GetHeight() });
		if (m_Controls)
		{
			m_pControlsTexture->Draw({ 0,0 }, { 0,0,m_pControlsTexture->GetWidth(), m_pControlsTexture->GetHeight() });
		}
	}
}


void Options::CheckIfOn(const SDL_MouseButtonEvent& e, std::shared_ptr<Avatar> avatar, std::shared_ptr<LootHandler> loothandler, Inventory& inventory, std::deque<std::shared_ptr<Enemy>>& pEnemies)
{
	//checks when to acitvate/deactivate certain parts of UI and changes all volume of sound
	if (utils::IsPointInRect({ float(e.x), float(600 - e.y) }, { 855, 330, 30,30 }))
	{
		if (m_IsOn == false)
		{
			m_IsOn = true;
			m_pOptionsSound->Play(0);
		}
	}
	if (m_IsOn)
	{
		if (utils::IsPointInRect({ float(e.x), float(600 - e.y) }, { 480, 65, 345,70 }))
		{
			if (m_Controls == false)
			{
				m_IsOn = false;
				m_pOptionsSound->Play(0);
			}
		}
		if (utils::IsPointInRect({ float(e.x), float(600 - e.y) }, { 50,60,280,70 }))
		{
			if (m_Controls == false)
			{
				m_Controls = true;
				m_pOptionsSound->Play(0);
			}
		}
		if (utils::IsPointInRect({ float(e.x), float(600 - e.y) }, { 585,15,150,85 }))
		{
			if (m_Controls)
			{
				m_Controls = false;
				m_pOptionsSound->Play(0);
			}
		}
		if (utils::IsPointInRect({ float(e.x), float(600 - e.y) }, { 215, 460, 225,40 }))
		{
			if (m_Controls == false)
			{
				m_pOptionsSound->Play(0);
				avatar->ChangeVolume(2);
				loothandler->ChangeVolume(2);
				inventory.ChangeVolume(2);
				for (std::shared_ptr<Enemy> element : pEnemies)
				{
					element->ChangeVolume(2);
				}
				m_Volume += 2;
			}
		}

		if (utils::IsPointInRect({ float(e.x), float(600 - e.y) }, { 455, 460, 225,40 }))
		{
			if (m_Controls == false)
			{
				m_pOptionsSound->Play(0);
				avatar->ChangeVolume(-2);
				loothandler->ChangeVolume(-2);
				inventory.ChangeVolume(-2);
				for (std::shared_ptr<Enemy> element : pEnemies)
				{
					element->ChangeVolume(-2);
				}
				m_Volume -= 2;
			}
		}
	}
}

void Options::ShowControls(const SDL_KeyboardEvent& e)
{
	m_IsOn = true;
	m_Controls = true;
}

bool Options::GiveActive()
{
	return m_IsOn;
}


