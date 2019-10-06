#include "stdafx.h"
#include "AvatarBullet.h"
#include "Enemy.h"

AvatarBullet::AvatarBullet(const Point2f& pos, int damage)
	:m_Center{pos}
	,m_BeginPoint{pos}
	,m_Velocity{200,200}
	,m_Angle{0}
	,m_IsActive{false}
	,m_Damage{damage}
	,m_pBulletTexture{ std::make_shared<Texture>("Resources/avatarbullet.png") }
{
}

void AvatarBullet::Draw() const
{
	if (m_IsActive == true)
	{
		glPushMatrix();
		glTranslatef(m_Center.x, m_Center.y, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(-m_Center.x, -m_Center.y, 0);
		m_pBulletTexture->Draw({ m_Center.x - m_pBulletTexture->GetWidth() / 2, m_Center.y - m_pBulletTexture->GetHeight() / 2 }, { 0,0,m_pBulletTexture->GetWidth(), m_pBulletTexture->GetHeight() });
		glPopMatrix();
	}
}

void AvatarBullet::Update(float elapsedSec, std::deque<std::shared_ptr<Enemy>>& pEnemies)
{
	m_Shape = { m_Center.x - m_pBulletTexture->GetWidth() / 2, m_Center.y - m_pBulletTexture->GetHeight() / 2 , m_pBulletTexture->GetWidth(),m_pBulletTexture->GetHeight() };
	if (m_IsActive)
	{
		CalculateVelocity();
		m_Center.x += m_Velocity.x * elapsedSec;
		m_Center.y += m_Velocity.y * elapsedSec;
	}
	CheckEnemiesHit(pEnemies);
}

void AvatarBullet::Shoot()
{
	m_IsActive = true;
}

void AvatarBullet::CalculateAngle(const Point2f& mousepos)
{
	//calculates angle between the mouse and avatar
	Vector2f xAxis{ 1,0 };
	Vector2f bulletLine{ m_Center , mousepos };
	m_Angle = xAxis.AngleWith(bulletLine) * 180 / float(M_PI);
}

void AvatarBullet::CalculateVelocity()
{
	m_Velocity.x = cos(m_Angle * float(M_PI) / 180) * 400;
	m_Velocity.y = sin(m_Angle * float(M_PI) / 180) * 400;
}

void AvatarBullet::SetPosition(const Point2f& pos)
{
	m_Center = pos;
}

Point2f AvatarBullet::GetPosition()
{
	return m_Center;
}

void AvatarBullet::GiveAngle(float angle)
{
	m_Angle = angle;
}

void AvatarBullet::CheckEnemiesHit(std::deque<std::shared_ptr<Enemy>>& pEnemies)
{
	//checks if bullet hit a enemy
	for (std::shared_ptr<Enemy> element: pEnemies)
	{
		if (m_IsActive)
		{
			if (element->IsDead() == false)
			{
				if (element->DoHitTest(m_Shape) == true)
				{
					m_IsActive = false;
					element->GetDamage(m_Damage);
				}
			}
		}
	}
}

void AvatarBullet::CheckOutOfRange(const Point2f& avatarPos)
{
	//checks when the bullet has to be deactivated
	Circlef rangeCircle(avatarPos.x, avatarPos.y, 288);
	if (utils::IsOverlapping(m_Shape, rangeCircle) == false)
	{
		m_IsActive = false;
	}
}

bool AvatarBullet::GetActive() const
{
	return m_IsActive;
}