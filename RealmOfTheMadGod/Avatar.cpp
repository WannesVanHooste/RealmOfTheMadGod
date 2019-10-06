#include "stdafx.h"
#include "Avatar.h"


Avatar::Avatar(Point2f pos, int health, int manna, int defence, float speed, int attack)
	:m_Position{pos}
	,m_Health{health}
	,m_Speed{speed,speed}
	,m_Manna{manna}
	,m_Defence{defence}
	,m_Attack{attack}
	,m_Level{1}
	,m_LevelConstant{0.15f}
	,m_Experience{0}
	,m_AvatarOrientation{AvatarOrientation::down}
	,m_AccuSet{0}
	,m_Actframe{0}
	,m_AccuSet1{ 0 }
	,m_Actframe1{ 0 }
	,m_Cols{5}
	,m_Rows{4}
	,m_FrameSec{0.2f}
	,m_HealthMax{100}
	,m_MannaMax{100}
	,m_MouseAngle{0}
	,m_PriorityMovement{false}
	,m_Shooting{false}
	,m_ShootDelay{false}
	,m_AccuSet2{0}
	,m_SpecialEnable{false}
	,m_AccuSetManna{0}
	,m_IsDead{false}
	,m_HorizontalVelocity{speed, speed}
	,m_VerticalVelocity{speed, speed}
	,m_MaxExperience{50}
	,m_AccuSet3{0}
	,m_SpecialDelay{false}
	,m_pAvatarTexture{ std::make_unique<Texture>("Resources/spritesheetavatar.png") }
	,m_pBulletSound{std::make_unique<SoundEffect>("Resources/Audio/Bullet.ogg")}
	,m_pSpecialSound{ std::make_unique<SoundEffect>("Resources/Audio/Fire_nova.ogg") }
	,m_pLevelUpSound{std::make_unique<SoundEffect>("Resources/Audio/Level_up.ogg")}
	,m_pNoMannaSound{ std::make_unique<SoundEffect>("Resources/Audio/No_mana.ogg") }
	,m_pHitSound{std::make_unique<SoundEffect>("Resources/Audio/Wizard_hit.ogg")}
	,m_pDeathSound{std::make_unique<SoundEffect>("Resources/Audio/Death_screen.ogg")}
	,m_Volume{10}
{
	//different volumes because some  sounds are louder than others or more important
	m_pBulletSound->SetVolume(6);
	m_pSpecialSound->SetVolume(10);
	m_pLevelUpSound->SetVolume(10);
	m_pNoMannaSound->SetVolume(10);
	m_pHitSound->SetVolume(10);
	m_pDeathSound->SetVolume(5);
}

void Avatar::Draw() const
{
	if (m_IsDead == false)
	{
		for (std::shared_ptr<AvatarBullet> element : m_pBullets)
		{
			element->Draw();
		}
		if (m_SpecialEnable == true)
		{
			for (std::shared_ptr<AvatarBullet> element : m_pSpecialAbility)
			{
				element->Draw();
			}
		}
		DrawTexture();
		DrawHealth();
	}
}

void Avatar::Update(float elapsedSec, std::deque<std::shared_ptr<Enemy>>& pEnemies)
{
	if (m_IsDead == false)
	{
		m_Shape = { m_Position.x - m_pAvatarTexture->GetWidth() / 5 / 2 + 15, m_Position.y - m_pAvatarTexture->GetHeight() / 4 / 2 + 14, m_pAvatarTexture->GetWidth() / 5 - 30, m_pAvatarTexture->GetHeight() / 4 - 30 };
		RegenHealth(elapsedSec);
		RegenManna(elapsedSec);
		CheckDead();
		Movement();
		LvlUp();
		UpdateTexture(elapsedSec);
		m_RayCast.Update(elapsedSec, m_Shape, m_HorizontalVelocity, m_VerticalVelocity, m_Speed);
		UpdateBullets(elapsedSec, pEnemies);
	}
}

void Avatar::SpecialAbility()
{
	//function that creates bullets for the special ability
	m_pSpecialSound->Play(0);
	for (int i{ 0 }; i < 12; ++i)
	{
		m_pSpecialAbility.push_back(std::make_shared<AvatarBullet>(m_Position, m_Attack));
	}
	for (int i{ 0 }; i < m_pSpecialAbility.size(); ++i)
	{
		m_pSpecialAbility[i]->GiveAngle(30 * float(i));
	}
}

void Avatar::LvlUp()
{
	//Functions that handles Xp and LvlUps that increase stats
	if (m_Level < 2)
	{
		if (m_Experience >= m_MaxExperience)
		{
			m_pLevelUpSound->Play(0);
			++m_Level;
			m_HealthMax += 25;
			m_Health = m_HealthMax;
			m_MannaMax += 10;
			m_Manna = m_MannaMax;
			m_Defence += 1;
			m_Experience = 0;
			m_MaxExperience = (m_Level - 1) * 100 + 50;
		}
	}
	//levelups that follow require more xp
	if (m_Level < 20 && m_Level > 1)
	{
		if (m_Experience >= m_MaxExperience)
		{
			m_pLevelUpSound->Play(0);
			++m_Level;
			m_HealthMax += 25;
			m_Health = m_HealthMax;
			m_MannaMax += 10;
			m_Manna = m_MannaMax;
			m_Defence += 1;
			m_Experience = 0;
			m_MaxExperience = (m_Level - 1) * 100 + 50;
		}
	}
}

void Avatar::RegenHealth(float elapsedSec)
{
	//function that regens 1hp every second
	if (m_Health < m_HealthMax)
	{
		m_AccuSetHealth += elapsedSec;
		if (m_AccuSetHealth >= 1)
		{
			m_Health += 1;
			m_AccuSetHealth = 0;
		}
	}
}

void Avatar::RegenManna(float elapsedSec)
{
	//function that regens 1manna every second
	if (m_Manna < m_MannaMax)
	{
		m_AccuSetManna += elapsedSec;
		if (m_AccuSetManna >= 1)
		{
			m_Manna += 1;
			m_AccuSetManna = 0;
		}
	}
}

void Avatar::Movement()
{
	//function that changes AvatarState, AvatarOrientation and handles the movements (and Special ability)
	const Uint8 *pKeyStates = SDL_GetKeyboardState(nullptr);
	if (pKeyStates[SDL_SCANCODE_A])
	{
		if (m_PriorityMovement == false)
		{
			m_AvatarOrientation = AvatarOrientation::left;
		}
		m_Position.x -= m_HorizontalVelocity.x;
	}
	if (pKeyStates[SDL_SCANCODE_D])
	{
		if (m_PriorityMovement == false)
		{
			m_AvatarOrientation = AvatarOrientation::right;
		}
		m_Position.x += m_HorizontalVelocity.y;
	}
	if (pKeyStates[SDL_SCANCODE_W])
	{
		if (m_PriorityMovement == false)
		{
			m_AvatarOrientation = AvatarOrientation::up;
		}
		m_Position.y += m_VerticalVelocity.x;
	}
	if (pKeyStates[SDL_SCANCODE_S])
	{
		if (m_PriorityMovement == false)
		{
			m_AvatarOrientation = AvatarOrientation::down;
		}
		m_Position.y -= m_VerticalVelocity.y;
	}

	//activates the special ability when enough manna is available
	if (m_SpecialDelay == true)
	{
		if (pKeyStates[SDL_SCANCODE_SPACE])
		{
			if (m_Manna >= 30)
			{
				if (m_Manna - 30 <= 0)
				{
					m_Manna = 0;
				}
				else {
					m_Manna -= 30;
				}
				SpecialAbility();
				m_SpecialEnable = true;
				for (std::shared_ptr<AvatarBullet> element : m_pSpecialAbility)
				{
					element->Shoot();
				}
			}
			else {
				m_pNoMannaSound->Play(0);
			}
			m_SpecialDelay = false;
		}
	}
}

void Avatar::UpdateTexture(float elapsedSec)
{
	//Updates the sprites and creates several bullet delays
	m_AccuSet += elapsedSec;
	if (m_AccuSet > m_FrameSec)
	{
		++m_Actframe;
		if (m_Actframe > 2)
		{
			m_Actframe = 0;
		}

		m_AccuSet -= m_FrameSec;
	}

	m_AccuSet1 += elapsedSec;
	if (m_AccuSet1 > m_FrameSec)
	{
		++m_Actframe1;
		if (m_Actframe1 > 1)
		{
			m_Actframe1 = 0;
		}

		m_AccuSet1 -= m_FrameSec;
	}

	//bulletdelay
	m_AccuSet2 += elapsedSec;
	if (m_AccuSet2 > 0.2)
	{
		m_ShootDelay = true;
		m_AccuSet2 = 0;
	}

	//special delay
	m_AccuSet3 += elapsedSec;
	if (m_AccuSet3 > 1.0f)
	{
		m_SpecialDelay = true;
		m_AccuSet3 = 0;
	}
}

void Avatar::DrawTexture() const
{
	//Draws certain animations dependable on the movement and shooting
	if (m_AvatarOrientation == AvatarOrientation::left)
	{
		m_pAvatarTexture->Draw({ m_Position.x  - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ 0 + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe),(m_pAvatarTexture->GetHeight() / m_Rows * 1), (m_pAvatarTexture->GetWidth() / m_Cols)
		, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
	else if (m_AvatarOrientation == AvatarOrientation::leftAttack)
	{
		m_pAvatarTexture->Draw({ m_Position.x - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ (m_pAvatarTexture->GetWidth() / m_Cols * 4) + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe1),(m_pAvatarTexture->GetHeight() / m_Rows * 1), (m_pAvatarTexture->GetWidth() / m_Cols)
			, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
	else if (m_AvatarOrientation == AvatarOrientation::right)
	{
		m_pAvatarTexture->Draw({ m_Position.x - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ 0 + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe),(m_pAvatarTexture->GetHeight() / m_Rows * 2), (m_pAvatarTexture->GetWidth() / m_Cols)
			, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
	else if (m_AvatarOrientation == AvatarOrientation::rightAttack)
	{
		m_pAvatarTexture->Draw({ m_Position.x - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ (m_pAvatarTexture->GetWidth() / m_Cols * 4) + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe1),(m_pAvatarTexture->GetHeight() / m_Rows * 2), (m_pAvatarTexture->GetWidth() / m_Cols)
			, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
	else if (m_AvatarOrientation == AvatarOrientation::down)
	{
		m_pAvatarTexture->Draw({ m_Position.x - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ 0 + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe),(m_pAvatarTexture->GetHeight() / m_Rows * 3), (m_pAvatarTexture->GetWidth() / m_Cols)
			, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
	else if (m_AvatarOrientation == AvatarOrientation::downAttack)
	{
		m_pAvatarTexture->Draw({ m_Position.x - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ (m_pAvatarTexture->GetWidth() / m_Cols * 4) + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe1),(m_pAvatarTexture->GetHeight() / m_Rows * 3), (m_pAvatarTexture->GetWidth() / m_Cols)
			, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
	else if (m_AvatarOrientation == AvatarOrientation::upAttack)
	{
		m_pAvatarTexture->Draw({ m_Position.x - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ 0 + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe),(m_pAvatarTexture->GetHeight() / m_Rows * 0), (m_pAvatarTexture->GetWidth() / m_Cols)
			, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
	else if (m_AvatarOrientation == AvatarOrientation::up)
	{
		m_pAvatarTexture->Draw({ m_Position.x - m_pAvatarTexture->GetWidth() / m_Cols / 2, m_Position.y - m_pAvatarTexture->GetHeight() / m_Rows / 2, m_pAvatarTexture->GetWidth() / m_Cols, m_pAvatarTexture->GetHeight() / m_Rows },
		{ (m_pAvatarTexture->GetWidth() / m_Cols * 4) + ((m_pAvatarTexture->GetWidth() / m_Cols) * m_Actframe1),(m_pAvatarTexture->GetHeight() / m_Rows * 0), (m_pAvatarTexture->GetWidth() / m_Cols)
			, (m_pAvatarTexture->GetHeight() / m_Rows) });
	}
}

void Avatar::ShootBullet(const SDL_MouseButtonEvent& e, std::shared_ptr<Camera> camera)
{
	//Function that shoots the bullet and changes the orientation according to the angle when clicking with mouse
	m_Shooting = true;
	Point2f mousePos{ float(e.x), float(600 - e.y) };
	mousePos.x += camera->GetPosition(GetShape()).x;
	mousePos.y += camera->GetPosition(GetShape()).y;
	if (m_ShootDelay == true)
	{
		if (m_IsDead == false)
		{
			m_pBulletSound->Play(0);
			m_pBullets.push_back(std::make_shared<AvatarBullet>(m_Position, m_Attack));
			m_pBullets.back()->CalculateAngle(mousePos);
			m_pBullets.back()->Shoot();
			m_ShootDelay = false;
		}
	}

	//checks angle and draws right sprite for direction
	float DegreeAngle{};
	m_MouseAngle = atan((mousePos.y - m_Position.y) / (mousePos.x - m_Position.x));
	DegreeAngle = m_MouseAngle * (180 / float(M_PI));
	if (mousePos.x > m_Position.x && DegreeAngle < 45 && DegreeAngle > -45)
	{
		m_PriorityMovement = true;
		m_AvatarOrientation = AvatarOrientation::rightAttack;
	}
	else if (mousePos.x < m_Position.x && DegreeAngle < 45 && DegreeAngle > -45)
	{
		m_PriorityMovement = true;
		m_AvatarOrientation = AvatarOrientation::leftAttack;
	}
	else if (mousePos.y > m_Position.y && (DegreeAngle < -45 || DegreeAngle > 45))
	{
		m_PriorityMovement = true;
		m_AvatarOrientation = AvatarOrientation::upAttack;
	}
	else if (mousePos.y < m_Position.y && (DegreeAngle < -45 || DegreeAngle > 45))
	{
		m_PriorityMovement = true;
		m_AvatarOrientation = AvatarOrientation::downAttack;
	}
}

void Avatar::Shooting(const SDL_MouseMotionEvent& e, std::shared_ptr<Camera> camera)
{
	//Function that shoots the bullet and changes the orientation according to the angle while moving with mouse
	if (m_Shooting == true)
	{
		Point2f mousePos{ float(e.x), float(600 - e.y) };
		mousePos.x += camera->GetPosition(GetShape()).x;
		mousePos.y += camera->GetPosition(GetShape()).y;
		if (m_ShootDelay == true)
		{
			if (m_IsDead == false)
			{
				m_pBulletSound->Play(0);
				m_pBullets.push_back(std::make_shared<AvatarBullet>(m_Position, m_Attack));
				m_pBullets.back()->CalculateAngle(mousePos);
				m_pBullets.back()->Shoot();
				m_ShootDelay = false;
			}
		}

		//checks angle and draws right sprite for direction
		float DegreeAngle{};
		m_MouseAngle = atan((mousePos.y - m_Position.y) / (mousePos.x - m_Position.x));
		DegreeAngle = m_MouseAngle * (180 / float(M_PI));
		if (mousePos.x > m_Position.x && DegreeAngle < 45 && DegreeAngle > -45)
		{
			m_PriorityMovement = true;
			m_AvatarOrientation = AvatarOrientation::rightAttack;
		}
		else if (mousePos.x < m_Position.x && DegreeAngle < 45 && DegreeAngle > -45)
		{
			m_PriorityMovement = true;
			m_AvatarOrientation = AvatarOrientation::leftAttack;
		}
		else if (mousePos.y > m_Position.y && (DegreeAngle < -45 || DegreeAngle > 45))
		{
			m_PriorityMovement = true;
			m_AvatarOrientation = AvatarOrientation::upAttack;
		}
		else if (mousePos.y < m_Position.y && (DegreeAngle < -45 || DegreeAngle > 45))
		{
			m_PriorityMovement = true;
			m_AvatarOrientation = AvatarOrientation::downAttack;
		}
	}
}

void Avatar::ResetPriority(const SDL_MouseButtonEvent& e)
{
	//Resets the AvatarOrientation
	m_Shooting = false;
	if (m_AvatarOrientation == AvatarOrientation::leftAttack)
	{
		m_AvatarOrientation = AvatarOrientation::left;
	}
	if (m_AvatarOrientation == AvatarOrientation::rightAttack)
	{
		m_AvatarOrientation = AvatarOrientation::right;
	}
	if (m_AvatarOrientation == AvatarOrientation::upAttack)
	{
		m_AvatarOrientation = AvatarOrientation::up;
	}
	if (m_AvatarOrientation == AvatarOrientation::downAttack)
	{
		m_AvatarOrientation = AvatarOrientation::down;
	}
	m_PriorityMovement = false;
}

Rectf Avatar::GetShape() const
{
	return m_Shape;
}

Point2f Avatar::GetPosition() const
{
	return m_Position;
}

bool Avatar::IsDead() const
{
	return m_IsDead;
}

void Avatar::CheckDead()
{
	if (m_Health <= 0)
	{
		m_pDeathSound->Play(0);
		m_IsDead = true;
	}
}

bool Avatar::DoHitTest(const Rectf& other)
{
	//Checks if the avatar is hit by enemy bullet
	if (utils::IsOverlapping(m_Shape, other))
	{
		m_pHitSound->Play(0);
		return true;
	}
	return false;
}

void Avatar::ReceiveDamage(int damage)
{
	if (damage - m_Defence > 0)
	{
		m_Health -= (damage - m_Defence);
	}
}

void Avatar::ReceiveExperience(int experience)
{
	m_Experience += experience;
}

int Avatar::DoDamage()
{
	return m_Attack;
}

void Avatar::DrawHealth() const
{
	//Function that draws the health underneath the avatar
	float ShowHealth{ (m_Shape.width / m_HealthMax) * m_Health };
	utils::SetColor(Color4f{ 0.7f, 0.7f, 0.7f, 1.0f });
	utils::FillRect(m_Shape.left, m_Shape.bottom - 10, m_Shape.width, m_Shape.height / 6);
	if (m_Health > m_HealthMax / 2)
	{
		utils::SetColor(Color4f{ 0.2f, 1.0f, 0.0f, 1.0f });
	}
	else if (m_Health < m_HealthMax / 2 && m_Health > m_HealthMax / 6)
	{
		utils::SetColor(Color4f{ 1.f, 0.5f, 0.08f, 1.0f });
	}
	else {
		utils::SetColor(Color4f{ 1.f, 0.0f, 0.0f, 1.0f });
	}
	utils::FillRect(m_Shape.left, m_Shape.bottom - 10, ShowHealth, m_Shape.height / 6);
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.0f });
	utils::DrawRect(m_Shape.left, m_Shape.bottom - 10, m_Shape.width, m_Shape.height / 6);
}

Point2f Avatar::GetHp() const
{
	return { float(m_Health), float(m_HealthMax) };
}

Point2f Avatar::GetManna() const
{
	return { float(m_Manna), float(m_MannaMax) };
}

Point2f Avatar::GetXp() const
{
	return { float(m_Experience), float(m_MaxExperience) };
}

int Avatar::GetLvl() const
{
	return m_Level;
}

void Avatar::ReceiveHp(int hp)
{
	//hp potions
	if (m_Health + hp >= m_HealthMax)
	{
		m_Health = m_HealthMax;
	}
	else {
		m_Health += hp;
	}
}

void Avatar::ReceiveManna(int manna)
{
	//mp potions
	if (m_Manna + manna >= m_MannaMax)
	{
		m_Manna = m_MannaMax;
	}
	else {
		m_Manna += manna;
	}
}

void Avatar::UpdateBullets(float elapsedSec, std::deque<std::shared_ptr<Enemy>>& pEnemies)
{
	//updates all the bullets of the avatar
	for (std::shared_ptr<AvatarBullet> element : m_pBullets)
	{
		element->Update(elapsedSec, pEnemies);
		element->CheckOutOfRange(m_Position);
	}
	for (std::shared_ptr<AvatarBullet> element : m_pSpecialAbility)
	{
		element->Update(elapsedSec, pEnemies);
		element->CheckOutOfRange(m_Position);
	}
	for (int i{}; i != m_pBullets.size(); ++i)
	{
		if (m_pBullets.size() > 1)
		{
			if (m_pBullets[i]->GetActive() == false)
			{
				m_pBullets[i].swap(m_pBullets[0]);
				m_pBullets.pop_front();
				i = 0;
			}
		}
	}
	for (int i{}; i != m_pSpecialAbility.size(); ++i)
	{
		if (m_pSpecialAbility.size() > 1)
		{
			if (m_pSpecialAbility[i]->GetActive() == false)
			{
				m_pSpecialAbility[i].swap(m_pSpecialAbility[0]);
				m_pSpecialAbility.pop_front();
				i = 0;
			}
		}
	}
}

void Avatar::ChangeVolume(int value)
{
	//Handles all the sounds in the avatar class
	if (m_Volume + value >= 0 && m_Volume + value <= 50)
	{
		m_Volume += value;
		m_pBulletSound->SetVolume(m_Volume - 4);
		m_pSpecialSound->SetVolume(m_Volume);
		m_pLevelUpSound->SetVolume(m_Volume);
		m_pNoMannaSound->SetVolume(m_Volume);
		m_pHitSound->SetVolume(m_Volume);
		m_pDeathSound->SetVolume(m_Volume);
		std::cout << "Volume is at: " << m_Volume << "\n";
	}
}