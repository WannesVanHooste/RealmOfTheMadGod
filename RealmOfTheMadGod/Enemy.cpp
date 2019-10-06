#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy(Point2f pos, int health, int damage, int experience, float speed, int enemyIndex)
	:m_Pos{pos}
	,m_Speed{speed}
	,m_Health{health}
	,m_State{EnemyState::idle}
	,m_IsDead{false}
	,m_Damage{damage}
	,m_ShootDelay{false}
	,m_AccuSet{0}
	,m_Experience{experience}
	,m_GiveExp{false}
	,m_HealthMax{health}
	,m_Index{enemyIndex}
	,m_Orientation{EnemyOrientation::left}
	,m_OutOfRange{false}
	,m_Volume{10}
	,m_pBulletSound{ std::make_unique<SoundEffect>("Resources/Audio/MagicShoot.ogg") }
{
	//texture differs on index given
	m_pEnemy = std::make_shared<Texture>( "Resources/enemy" + std::to_string(enemyIndex) + ".png" );
	m_pBulletSound->SetVolume(10);
}

void Enemy::Draw() const
{
	for (std::shared_ptr<EnemyBullet> element : m_pBullets)
	{
		element->Draw();
	}
	if (m_IsDead == false)
	{
		if (m_OutOfRange == false)
		{
			if (m_Orientation == EnemyOrientation::right)
			{
				m_pEnemy->Draw({ m_Pos.x - m_pEnemy->GetWidth() / 2, m_Pos.y - m_pEnemy->GetHeight() / 2 }, { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() });
			}
			else if (m_Orientation == EnemyOrientation::left)
			{
				glPushMatrix();
				glTranslatef(m_Shape.left, m_Shape.bottom, 0);
				glScalef(-1, 1, 1);
				glTranslatef(-m_Shape.left - m_pEnemy->GetWidth(), -m_Shape.bottom, 0);
				m_pEnemy->Draw({ m_Pos.x - m_pEnemy->GetWidth() / 2, m_Pos.y - m_pEnemy->GetHeight() / 2 }, { 0,0,m_pEnemy->GetWidth(), m_pEnemy->GetHeight() });
				glPopMatrix();
			}
		}
		DrawHealth();
	}
}

void Enemy::Update(float elapsedSec, std::shared_ptr<Avatar> avatar)
{
	CheckDead();
	m_Shape = { m_Pos.x - m_pEnemy->GetWidth() / 2, m_Pos.y - m_pEnemy->GetHeight() / 2, m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
	for (std::shared_ptr<EnemyBullet> element : m_pBullets)
	{
		element->Update(elapsedSec, avatar);
		element->CheckOutOfRange(m_Pos);
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
	if (m_IsDead == false)
	{
		CheckState(avatar);
		if (m_State == EnemyState::idle)
		{
			// random movements
			RandomMoves();
		}
		if (m_State == EnemyState::triggered)
		{
			if (avatar->IsDead() == false)
			{
				Shoot(avatar);
				// run towards avatar
				RandomMoves();
				Vector2f bulletLine{ m_Pos , avatar->GetPosition() };
				bulletLine = bulletLine.Normalized();
				if (GetDistance(avatar) > 220)
				{
					m_Pos.x += bulletLine.x * m_Speed;
					m_Pos.y += bulletLine.y * m_Speed;
				}
				if (bulletLine.x < 0)
				{
					m_Orientation = EnemyOrientation::left;
				}
				else {
					m_Orientation = EnemyOrientation::right;
				}
			}
		}

		//bulletdelays
		m_AccuSet += elapsedSec;
		if (m_AccuSet > 0.5)
		{
			m_ShootDelay = true;
			m_AccuSet = 0;
		}
		if (m_Pos.x - m_pEnemy->GetWidth() / 2 - avatar->GetPosition().x > 300)
		{
			m_OutOfRange = true;
		}
		else {
			m_OutOfRange = false;
		}
	}
	GiveExperience(avatar);
}

bool Enemy::DoHitTest(const Rectf& other)
{
	//checks if enemy got hit by avatar bullet
	if (utils::IsOverlapping(other, m_Shape))
	{
		return true;
	}
	return false;
}

void Enemy::Shoot(std::shared_ptr<Avatar> avatar)
{
	if (m_ShootDelay)
	{
		m_pBulletSound->Play(0);
		m_pBullets.push_back(std::make_shared<EnemyBullet>(m_Pos, m_Damage,  m_Index));
		m_pBullets.back()->CalculateAngle(avatar);
		m_ShootDelay = false;
	}
}

float Enemy::GetDistance(std::shared_ptr<Avatar> avatar)
{
	//calculates distance between avatar and enemy
	return sqrt(pow(m_Pos.x - avatar->GetPosition().x, 2) + pow(m_Pos.y - avatar->GetPosition().y, 2));
}

void Enemy::SetShape()
{
	m_Shape = { m_Pos.x - m_pEnemy->GetWidth() / 2, m_Pos.y - m_pEnemy->GetHeight() / 2, m_pEnemy->GetWidth(), m_pEnemy->GetHeight() };
}

bool Enemy::IsDead() const
{
	return m_IsDead;
}

void Enemy::CheckDead()
{
	if (m_Health <= 0)
	{
		m_IsDead = true;
	}
}

void Enemy::CheckState(std::shared_ptr<Avatar> avatar)
{
	if (GetDistance(avatar) < 300)
	{
		m_State = EnemyState::triggered;
	}
	if (GetDistance(avatar) > 330) //more difficult to lose an enemy than to trigger one
	{
		m_State = EnemyState::idle;
	}
}

void Enemy::GetDamage(int damage)
{
	//enemy receives damage and is triggered when hit
	m_Health -= damage;
	m_State = EnemyState::triggered;
}

void Enemy::GiveExperience(std::shared_ptr<Avatar> avatar)
{
	//gives experience to the avatar when enemy is dead
	if (m_IsDead)
	{
		if (m_GiveExp == false)
		{
			avatar->ReceiveExperience(m_Experience);
			m_GiveExp = true;
		}
	}
}

void Enemy::DrawHealth() const
{
	if (m_OutOfRange == false)
	{
		float ShowHealth{ (m_Shape.width / m_HealthMax) * m_Health };
		utils::SetColor(Color4f{ 0.7f, 0.7f, 0.7f, 1.0f });
		utils::FillRect(m_Shape.left, m_Shape.bottom - 10, m_Shape.width, m_Shape.height / 10);
		if (m_Health > m_HealthMax / 2)
		{
			utils::SetColor(Color4f{ 0.2f, 1.0f, 0.0f, 1.0f });
		}
		else if (m_Health <= m_HealthMax / 2 && m_Health > m_HealthMax / 6)
		{
			utils::SetColor(Color4f{ 1.f, 0.5f, 0.08f, 1.0f });
		}
		else {
			utils::SetColor(Color4f{ 1.f, 0.0f, 0.0f, 1.0f });
		}
		utils::FillRect(m_Shape.left, m_Shape.bottom - 10, ShowHealth, m_Shape.height / 10);
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.0f });
		utils::DrawRect(m_Shape.left, m_Shape.bottom - 10, m_Shape.width, m_Shape.height / 10);
	}
}

void Enemy::RandomMoves()
{
	//Random Move generator
	int randomSpeed = rand() % (20 - 8) + 8;
	float randomSpeed1 = randomSpeed / 20.0f;
	int randomdirection = rand() % 4;
	if (randomdirection == 0)
	{
		m_Pos.x += randomSpeed1;
	}
	else if (randomdirection == 1)
	{
		m_Pos.x -= randomSpeed1;
	}
	else if (randomdirection == 2)
	{
		m_Pos.y += randomSpeed1;
	}
	else if (randomdirection == 3)
	{
		m_Pos.y -= randomSpeed1;
	}
}

Point2f Enemy::GetPosition() const
{
	return m_Pos;
}

int Enemy::GetIndex()
{
	return m_Index;
}

void Enemy::ChangeVolume(int value)
{
	if (m_Volume + value >= 0 && m_Volume + value <= 50)
	{
		m_Volume += value;
		m_pBulletSound->SetVolume(m_Volume);
	}
}
