#pragma once
#include "Avatar.h"

class EnemyBullet
{
public:
	EnemyBullet(const Point2f& pos, int damage, int bulletIndex);
	void Draw() const;
	void Update(float elapsedSec, std::shared_ptr<Avatar> avatar);
	void CalculateAngle(std::shared_ptr<Avatar> avatar);
	void SetPosition(const Point2f& pos);
	void GiveAngle(float angle);
	void CheckAvatarHit(std::shared_ptr<Avatar> avatar);
	Point2f GetPosition();
	void CheckOutOfRange(const Point2f& enemyPos);
	bool GetActive() const;
private:
	Point2f m_Center;
	std::shared_ptr<Texture> m_pBulletTexture;
	Vector2f m_Velocity;
	float m_Angle;
	void CalculateVelocity();
	bool m_IsActive;
	int m_Damage;
	Point2f m_BeginPoint;
	Rectf m_Shape;
};

