#include "stdafx.h"
#include "Vegetation.h"


Vegetation::Vegetation()
	:m_Input{ "Resources/Vegetation.txt" }
	,m_Height{6336}
	,m_pBush{ std::make_shared<Texture>("Resources/bush1.png") }
	,m_pCactus{ std::make_shared<Texture>("Resources/cactus1.png") }
	,m_pRock{ std::make_shared<Texture>("Resources/rock1.png") }
	,m_pTree1{ std::make_shared<Texture>("Resources/tree1.png") }
	,m_pTree2{ std::make_shared<Texture>("Resources/tree2.png") }
	,m_pTree3{ std::make_shared<Texture>("Resources/tree3.png") }
	,m_pTree4{ std::make_shared<Texture>("Resources/tree4.png") }
{
	ReadFile();
}

void Vegetation::Update(float elapsedSec, std::shared_ptr<Avatar> avatar)
{
	for (int i{ 0 }; i < 99; ++i)
	{
		for (int j{ 0 }; j < 99; ++j)
		{
			if (m_Height - float(j * 64) <= avatar->GetShape().bottom)
			{
				m_DrawUnder[i][j] = true;
			}
			else
			{
				m_DrawUnder[i][j] = false;
			}
		}
	}
}

void Vegetation::Draw(std::shared_ptr<Avatar> avatar) const
{
	//draws all the vegetation thats above the avatar
	for (int i{ 0 }; i < 99; ++i)
	{
		for (int j{ 0 }; j < 99; ++j)
		{
			if (m_DrawUnder[i][j] == true)
			{
				if (m_Vegetation[i][j] == Vegetation::bush)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pBush->Draw({ float(i * 64), m_Height - float(j * 64) }, { 0,0,m_pBush->GetWidth(), m_pBush->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::cactus)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pCactus->Draw({ float(i * 64), m_Height - float(j * 64) }, { 0,0,m_pCactus->GetWidth(), m_pCactus->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::rock)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pRock->Draw({ float(i * 64), m_Height - float(j * 64) }, { 0,0,m_pRock->GetWidth(), m_pRock->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree1)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree1->Draw({ float(i * 64), m_Height - float(j * 64), m_pTree1->GetWidth() * 1.5f, m_pTree1->GetHeight() * 1.5f }, { 0,0,m_pTree1->GetWidth(), m_pTree1->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree2)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree2->Draw({ float(i * 64), m_Height - float(j * 64), m_pTree2->GetWidth() * 1.5f, m_pTree2->GetHeight() * 1.5f }, { 0,0,m_pTree2->GetWidth(), m_pTree2->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree3)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree3->Draw({ float(i * 64), m_Height - float(j * 64), m_pTree3->GetWidth() * 1.10f, m_pTree3->GetHeight() * 1.10f }, { 0,0,m_pTree3->GetWidth(), m_pTree3->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree4)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree4->Draw({ float(i * 64), m_Height - float(j * 64), m_pTree4->GetWidth() * 1.10f, m_pTree4->GetHeight() * 1.10f }, { 0,0,m_pTree4->GetWidth(), m_pTree4->GetHeight() });
					}
				}
			}
		}
	}
}

void Vegetation::DrawUnder(std::shared_ptr<Avatar> avatar) const
{
	//draws everything thats under the avatar
	for (int i{ 0 }; i < 99; ++i)
	{
		for (int j{ 0 }; j < 99; ++j)
		{
			if (m_DrawUnder[i][j] == false)
			{
				if (m_Vegetation[i][j] == Vegetation::bush)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pBush->Draw({ float(i * 64), m_Height - float(j * 64) }, { 0,0,m_pBush->GetWidth(), m_pBush->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::cactus)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pCactus->Draw({ float(i * 64), m_Height - float(j * 64) }, { 0,0,m_pCactus->GetWidth(), m_pCactus->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::rock)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pRock->Draw({ float(i * 64), m_Height - float(j * 64) }, { 0,0,m_pRock->GetWidth(), m_pRock->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree1)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree1->Draw({ float(i * 64), m_Height - float(j * 64), m_pTree1->GetWidth() * 1.5f, m_pTree1->GetHeight() * 1.5f }, { 0,0,m_pTree1->GetWidth(), m_pTree1->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree2)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree2->Draw({ float(i * 64), m_Height - float(j * 64), m_pTree2->GetWidth() * 1.5f, m_pTree2->GetHeight() * 1.5f }, { 0,0,m_pTree2->GetWidth(), m_pTree2->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree3)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree3->Draw({ float(i * 64), m_Height - float(j * 64),  m_pTree3->GetWidth() * 1.10f, m_pTree3->GetHeight() * 1.10f }, { 0,0,m_pTree3->GetWidth(), m_pTree3->GetHeight() });
					}
				}
				else if (m_Vegetation[i][j] == Vegetation::tree4)
				{
					if (float(i * 64) - avatar->GetPosition().x <= 300)
					{
						m_pTree4->Draw({ float(i * 64), m_Height - float(j * 64), m_pTree4->GetWidth() * 1.10f, m_pTree4->GetHeight() * 1.10f }, { 0,0,m_pTree4->GetWidth(), m_pTree4->GetHeight() });
					}
				}
			}
		}
	}
}

void Vegetation::ReadFile()
{
	//reads from the text file where vegetation has to appear
	int i{0};
	int j{ 0 };
	std::string imputString;
	while (std::getline(m_Input, imputString, ','))
	{
		if (imputString == "s")
		{
			m_Vegetation[i][j] = VegetationState::none;
			++i;
		}
		else if (imputString == "z")
		{
			m_Vegetation[i][j] = VegetationState::none;
			++i;
		}
		else if (imputString == "g")
		{
			m_Vegetation[i][j] = VegetationState::none;
			++i;
		}
		else if (imputString == "d")
		{
			m_Vegetation[i][j] = VegetationState::none;
			++i;
		}
		else if (imputString == "w")
		{
			m_Vegetation[i][j] = VegetationState::none;
			++i;
		}
		else if (imputString == "b")
		{
			m_Vegetation[i][j] = VegetationState::none;
			++i;
		}
		else if (imputString == "1")
		{
			m_Vegetation[i][j] = VegetationState::bush;
			++i;
		}
		else if (imputString == "2")
		{
			m_Vegetation[i][j] = VegetationState::cactus;
			++i;
		}
		else if (imputString == "3")
		{
			m_Vegetation[i][j] = VegetationState::rock;
			++i;
		}
		else if (imputString == "4")
		{
			m_Vegetation[i][j] = VegetationState::tree1;
			++i;
		}
		else if (imputString == "5")
		{
			m_Vegetation[i][j] = VegetationState::tree2;
			++i;
		}
		else if (imputString == "6")
		{
			m_Vegetation[i][j] = VegetationState::tree3;
			++i;
		}
		else if (imputString == "7")
		{
			m_Vegetation[i][j] = VegetationState::tree4;
			++i;
		}

		if (i == 99)
		{
			++j;
			i = 0;
		}
	}
}
