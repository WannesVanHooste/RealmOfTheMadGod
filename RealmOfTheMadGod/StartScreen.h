#pragma once
#include "Texture.h"
#include <memory>
#include "utils.h"
#include "SoundEffect.h"
#include "SoundStream.h"

class StartScreen
{
public:
	StartScreen();
	void Draw() const;
	void Update(float elapsedSec);
	void CheckIfOn(const SDL_MouseButtonEvent& e);
	bool GiveActive();
private:
	std::unique_ptr<Texture> m_pStartscreenTexture;
	bool m_IsActive;
	std::unique_ptr<SoundEffect> m_pPressStartSound;
	std::unique_ptr<SoundStream> m_pSoundTrack;
};

