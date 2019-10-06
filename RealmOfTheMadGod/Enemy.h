#pragma once
#include "EnemyBullet.h"
#include "SoundEffect.h"

class EnemyBullet;

class Enemy
{
public:
	enum EnemyState {
		idle,
		triggered,
		dead
	};
	enum EnemyOrientation {
		left,
		right
	};
	Enemy(Point2f pos, int health, int damage, int experience, float speed, int enemyIndex);
	void Draw() const;
	void Update(float elapsedSec, std::shared_ptr<Avatar> avatar);
	void Shoot(std::shared_ptr<Avatar> avatar);
	bool DoHitTest(const Rectf& other);
	bool IsDead() const;
	void CheckState(std::shared_ptr<Avatar> avatar);
	float GetDistance(std::shared_ptr<Avatar> avatar);
	void GetDamage(int damage);
	Point2f GetPosition() const;
	int GetIndex();
	void ChangeVolume(int value);
private:
	int m_Index;
	float m_Speed;
	int m_HealthMax;
	bool m_IsDead;
	std::shared_ptr<Texture> m_pEnemy;
	Point2f m_Pos;
	int m_Health;
	EnemyState m_State;
	Rectf m_Shape;
	int m_Damage;
	int m_Experience;
	bool m_ShootDelay;
	float m_AccuSet;
	bool m_OutOfRange;
	int m_Volume;
	EnemyOrientation m_Orientation;
	std::deque<std::shared_ptr<EnemyBullet>> m_pBullets;
	std::unique_ptr<SoundEffect> m_pBulletSound;
	void SetShape();
	void CheckDead();
	void GiveExperience(std::shared_ptr<Avatar> avatar);
	bool m_GiveExp;
	void DrawHealth() const;
	void RandomMoves();
};

