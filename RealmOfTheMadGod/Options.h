#pragma once
#include "LootHandler.h"

class Options
{
public:
	Options();
	void Draw() const;
	void CheckIfOn(const SDL_MouseButtonEvent& e, std::shared_ptr<Avatar> avatar, std::shared_ptr<LootHandler> loothandler, Inventory& inventory, std::deque<std::shared_ptr<Enemy>>& pEnemies);
	bool GiveActive();
	void ShowControls(const SDL_KeyboardEvent& e);
private:
	bool m_IsOn;
	std::unique_ptr<Texture> m_pOptionsTexture;
	std::unique_ptr<SoundEffect> m_pOptionsSound;
	std::unique_ptr<Texture> m_pControlsTexture;
	bool m_Controls;
	int m_Volume;

};

