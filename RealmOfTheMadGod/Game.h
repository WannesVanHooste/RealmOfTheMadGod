#pragma once
#include <iostream>
#include "Avatar.h"
#include "Vegetation.h"
#include "HUD.h"
#include "Options.h"
#include "StartScreen.h"

class Game
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;
	std::shared_ptr<Avatar> m_pAvatar;
	std::shared_ptr<Texture> m_pMapTexture;
	std::deque<std::shared_ptr<Enemy>> m_pEnemies;
	std::shared_ptr<Camera> m_pCamera;
	std::unique_ptr<Vegetation> m_pVegetation;
	std::unique_ptr<HUD> m_pHud;
	std::shared_ptr<LootHandler> m_pLootHandler;
	std::unique_ptr<Options> m_pOptions;
	std::unique_ptr<StartScreen> m_pStartScreen;
	Inventory m_Inventory;

	// FUNCTIONS
	void Initialize( );
	void ClearBackground( ) const;
	void InitializeEnemies();
	void RemoveEnemies();
	void SpawnEnemies(int index);
};