#include "stdafx.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:m_Window{ window }
	,m_pCamera{std::make_shared<Camera>(600.f,600.f)}
	,m_pMapTexture{ std::make_shared<Texture>("Resources/map.png") }
	,m_pVegetation{std::make_unique<Vegetation>()}
	,m_pHud{ std::make_unique<HUD>(Point2f{600.f,0.f}) }
	,m_pLootHandler{std::make_shared<LootHandler>()}
	,m_Inventory{}
	,m_pOptions{std::make_unique<Options>()}
	,m_pAvatar{ std::make_shared<Avatar>(Point2f{ 1200 , 1000 }, 100,100,0,3.f, 20) }
	,m_pStartScreen{std::make_unique<StartScreen>()}
{
	Initialize( );
}

void Game::Initialize()
{
	InitializeEnemies();
}

void Game::Update( float elapsedSec )
{
	if (m_pStartScreen->GiveActive() == false)
	{
		if (m_pOptions->GiveActive() == false)
		{
			m_pAvatar->Update(elapsedSec, m_pEnemies);
			for (std::shared_ptr<Enemy> element : m_pEnemies)
			{
				element->Update(elapsedSec, m_pAvatar);
			}
			m_pVegetation->Update(elapsedSec, m_pAvatar);
			m_pLootHandler->Update(elapsedSec, m_pEnemies, m_pAvatar, m_Inventory);
			RemoveEnemies();
			m_Inventory.Update(elapsedSec, m_pAvatar);
		}
	}
	m_pStartScreen->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_pHud->DrawMiniMap(m_pAvatar, m_pEnemies, m_pMapTexture);
	glPushMatrix();
	glTranslatef(-m_pCamera->GetPosition(m_pAvatar->GetShape()).x, -m_pCamera->GetPosition(m_pAvatar->GetShape()).y, 0);
	m_pMapTexture->Draw({ 0,0 }, { 0,0,m_pMapTexture->GetWidth() - 6060 + m_pAvatar->GetPosition().x , m_pMapTexture->GetHeight()});
	m_pLootHandler->DrawBags();
	m_pVegetation->DrawUnder(m_pAvatar);
	m_pAvatar->Draw();
	m_pVegetation->Draw(m_pAvatar);
	for (std::shared_ptr<Enemy> element : m_pEnemies)
	{
		element->Draw();
	}
	glPopMatrix();
	m_pHud->Draw(m_pAvatar);
	m_pLootHandler->DrawItems(m_pAvatar);
	m_Inventory.Draw();
	m_pOptions->Draw();
	m_pStartScreen->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_i:
		m_pOptions->ShowControls(e);
		break;
	case SDLK_o:
		m_pOptions->ShowControls(e);
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	if (m_pOptions->GiveActive() == false)
	{
		m_pAvatar->Shooting(e, m_pCamera);
	}
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (m_pOptions->GiveActive() == false)
	{
		if (e.x < 600)
		{
			m_pAvatar->ShootBullet(e, m_pCamera);
		}
	}
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	m_pAvatar->ResetPriority(e);
	if (m_pStartScreen->GiveActive())
	{
		m_pStartScreen->CheckIfOn(e);
	}
	if (m_pStartScreen->GiveActive() == false)
	{
		m_pOptions->CheckIfOn(e, m_pAvatar, m_pLootHandler, m_Inventory, m_pEnemies);
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::RemoveEnemies()
{
	for (int i{ 0 }; i != m_pEnemies.size(); ++i)
	{
		if (m_pEnemies.size() > 1)
		{
			if (m_pEnemies[i]->IsDead() == true)
			{
				SpawnEnemies(m_pEnemies[i]->GetIndex());
				m_pEnemies[i].swap(m_pEnemies[0]);
				m_pEnemies.pop_front();
				i = 0;
			}
		}
	}
}

void Game::SpawnEnemies(int index)
{
	//create random spawn
	Point2f RandomPoint{ float(rand() % 6400), float(rand() % 6400) };
	Rectf rect1{ 1000, 1500, 4600, 4000 };
	Rectf rect2{ 1500,2000,3600,3000 };
	Rectf rect3{ 2000,2500,2600,2000 };
	Rectf rect4{ 2500,3000,1600,1000 };
	if (index < 2)
	{
		while (utils::IsPointInRect(RandomPoint, rect1) == false || utils::IsPointInRect(RandomPoint, rect2) == true)
		{
			RandomPoint = { float(rand() % 6400), float(rand() % 6400) };
		}
	}
	else if (index > 1 && index < 4)
	{
		while (utils::IsPointInRect(RandomPoint, rect2) == false || utils::IsPointInRect(RandomPoint, rect3) == true)
		{
			RandomPoint = { float(rand() % 6400), float(rand() % 6400) };
		}
	}
	else  if (index > 3 && index < 6)
	{
		while (utils::IsPointInRect(RandomPoint, rect3) == false || utils::IsPointInRect(RandomPoint, rect4) == true)
		{
			RandomPoint = { float(rand() % 6400), float(rand() % 6400) };
		}
	}
	else {
		while (utils::IsPointInRect(RandomPoint, rect4) == false)
		{
			RandomPoint = { float(rand() % 6400), float(rand() % 6400) };
		}
	}


	if (index == 0)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint }, 22, 9, 3, 3.f, 0));
	}
	else if (index == 1)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint }, 88, 13, 9, 2.8f, 1));
	}
	else if (index == 2)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint }, 400, 35, 40, 2.2f, 2));
	}
	else if (index == 3)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint }, 1270, 30, 228, 1.2f, 3));
	}
	else if (index == 4)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint }, 800, 30, 320, 2.5f, 4));
	}
	else if (index == 5)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint}, 1400, 60, 280, 2.3f, 5));
	}
	else if (index == 6)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint }, 1000, 70, 100, 1.9f, 6));
	}
	else if (index == 7)
	{
		m_pEnemies.push_back(std::make_shared<Enemy>(Point2f{ RandomPoint }, 1500, 120, 150, 1.5f, 7));
	}
}

void Game::InitializeEnemies()
{
	for (int i{ 0 }; i < 9; ++i)
	{
		if (i < 2)
		{
			for (int j{}; j < 10; ++j)
			{
				SpawnEnemies(i);
			}
		}
		else if (i < 4 && i > 1)
		{
			for (int j{}; j < 8; ++j)
			{
				SpawnEnemies(i);
			}
		}
		else if (i < 6 && i > 3)
		{
			SpawnEnemies(i);
			SpawnEnemies(i);
			SpawnEnemies(i);
		}
		else {
			SpawnEnemies(i);
			SpawnEnemies(i);
		}
	}
}