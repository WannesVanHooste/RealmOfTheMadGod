#pragma once
#include "Texture.h"
#include "Vector2f.h"
#include <deque>
#include <memory>

class Enemy;

class AvatarBullet
{
public:
	AvatarBullet(const Point2f& pos, int damage);
	void Draw() const;
	void Update(float elapsedSec, std::deque<std::shared_ptr<Enemy>>& pEnemies);
	void Shoot();
	void CalculateAngle(const Point2f& mousepos);
	void SetPosition(const Point2f& pos);
	void GiveAngle(float angle);
	void CheckEnemiesHit(std::deque<std::shared_ptr<Enemy>>& pEnemies);
	void CheckOutOfRange(const Point2f& avatarPos);
	bool GetActive() const;
	Point2f GetPosition();
private:
	Point2f m_Center;
	std::shared_ptr<Texture> m_pBulletTexture;
	Vector2f m_Velocity;
	float m_Angle;
	void CalculateVelocity();
	bool m_IsActive;
	Point2f m_BeginPoint;
	Rectf m_Shape;
	int m_Damage;
};

