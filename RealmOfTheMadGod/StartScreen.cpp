#include "stdafx.h"
#include "StartScreen.h"


StartScreen::StartScreen()
	:m_pStartscreenTexture{std::make_unique<Texture>("Resources/Startscreen.png")}
	,m_IsActive{true}
	,m_pPressStartSound{std::make_unique<SoundEffect>("Resources/Audio/Options.ogg")}
	,m_pSoundTrack{std::make_unique<SoundStream>("Resources/Audio/Soundtrack.mp3")}
{
	m_pPressStartSound->SetVolume(10);
	m_pSoundTrack->SetVolume(10);
	m_pSoundTrack->Play(true);
}

void StartScreen::Draw() const
{
	if (m_IsActive)
	{
		m_pStartscreenTexture->Draw({ 0,0 }, { 0,0,m_pStartscreenTexture->GetWidth(), m_pStartscreenTexture->GetHeight() });
	}
}

void StartScreen::Update(float elapsedSec)
{
	if (m_IsActive == false)
	{
		m_pSoundTrack->Stop();
	}
}

void StartScreen::CheckIfOn(const SDL_MouseButtonEvent& e)
{
	//checks when the startscreen has to disappear
	if (utils::IsPointInRect({ float(e.x), float(600 - e.y) }, { 265, 220, 345,70 }))
	{
		m_IsActive = false;
		m_pPressStartSound->Play(0);
	}
}

bool StartScreen::GiveActive()
{
	return m_IsActive;
}

