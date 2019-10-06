#include "stdafx.h"
#include "EnemyBullet.h"


EnemyBullet::EnemyBullet(const Point2f& pos, int damage, int bulletIndex)
	:m_Center{ pos }
	, m_BeginPoint{ pos }
	, m_Velocity{ 100,100 }
	, m_Angle{ 0 }
	, m_IsActive{ true }
	,m_Damage{damage}
{
	//texture differs for each type of enemy
	m_pBulletTexture = std::make_shared<Texture>( "Resources/bullet" + std::to_string(bulletIndex) + ".png" );
}

void EnemyBullet::Draw() const
{
	if (m_IsActive == true)
	{
		glPushMatrix();
		glTranslatef(m_Center.x, m_Center.y, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(-m_Center.x, -m_Center.y, 0);
		m_pBulletTexture->Draw({ m_Shape.left, m_Shape.bottom }, { 0,0,m_Shape.width, m_Shape.height });
		glPopMatrix();
	}
}

void EnemyBullet::Update(float elapsedSec, std::shared_ptr<Avatar> avatar)
{
	m_Shape = { m_Center.x - m_pBulletTexture->GetWidth() / 2, m_Center.y - m_pBulletTexture->GetHeight() / 2 , m_pBulletTexture->GetWidth(),m_pBulletTexture->GetHeight() };
	if (m_IsActive == true)
	{
		CalculateVelocity();
		m_Center.x += m_Velocity.x * elapsedSec;
		m_Center.y += m_Velocity.y * elapsedSec;
	}
	CheckAvatarHit(avatar);
}

void EnemyBullet::CalculateAngle(std::shared_ptr<Avatar> avatar)
{
	//calculates angle between avatar and enemy
	Vector2f xAxis{ 1,0 };
	Vector2f bulletLine{ m_Center , avatar->GetPosition() };
	m_Angle = xAxis.AngleWith(bulletLine) * 180 / float(M_PI);
}

void EnemyBullet::CalculateVelocity()
{
	m_Velocity.x = cos(m_Angle * float(M_PI) / 180) * 300;
	m_Velocity.y = sin(m_Angle * float(M_PI) / 180) * 300;
}

void EnemyBullet::SetPosition(const Point2f& pos)
{
	m_Center = pos;
}

Point2f EnemyBullet::GetPosition()
{
	return m_Center;
}

void EnemyBullet::GiveAngle(float angle)
{
	m_Angle = angle;
}

void EnemyBullet::CheckAvatarHit(std::shared_ptr<Avatar> avatar)
{
	if (m_IsActive)
	{
		if (avatar->IsDead() == false)
		{
			if (avatar->DoHitTest(m_Shape))
			{
				avatar->ReceiveDamage(m_Damage);
				m_IsActive = false;
			}
		}
	}
}

void EnemyBullet::CheckOutOfRange(const Point2f& enemyPos)
{
	Circlef rangeCircle(enemyPos.x, enemyPos.y, 288);
	if (utils::IsOverlapping(m_Shape, rangeCircle) == false)
	{
		m_IsActive = false;
	}
}

bool EnemyBullet::GetActive() const
{
	return m_IsActive;
}