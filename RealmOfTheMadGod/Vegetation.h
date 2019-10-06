#pragma once
#include <iostream>
#include <fstream>
#include "Avatar.h"

class Vegetation
{
public:
	enum VegetationState {
		bush,
		rock,
		cactus,
		tree1,
		tree2,
		tree3,
		tree4,
		none
	};
	void Draw(std::shared_ptr<Avatar> avatar) const;
	void DrawUnder(std::shared_ptr<Avatar> avatar) const;
	void Update(float elapsedSec, std::shared_ptr<Avatar> avatar);
	void ReadFile();
	Vegetation();
private:
	VegetationState m_Vegetation[100][100];
	std::shared_ptr<Texture> m_pBush;
	std::shared_ptr<Texture> m_pRock;
	std::shared_ptr<Texture> m_pCactus;
	std::shared_ptr<Texture> m_pTree1;
	std::shared_ptr<Texture> m_pTree2;
	std::shared_ptr<Texture> m_pTree3;
	std::shared_ptr<Texture> m_pTree4;
	std::ifstream m_Input;
	float m_Height;
	bool m_DrawUnder[100][100];
};

