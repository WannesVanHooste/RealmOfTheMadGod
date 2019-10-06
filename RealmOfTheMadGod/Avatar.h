#pragma once
#include "Texture.h"
#include "utils.h"
#include "AvatarBullet.h"
#include <deque>
#include "Camera.h"
#include "RayCast.h"
#include "Enemy.h"
#include <memory>
#include "SoundEffect.h"

class Avatar
{
public:
	enum AvatarOrientation {
		left,
		right,
		down,
		up,
		leftAttack,
		rightAttack,
		upAttack,
		downAttack
	};
	Avatar(Point2f pos, int health, int manna, int defence, float speed, int attack);
	void Draw() const;
	void Update(float elapsedSec, std::deque<std::shared_ptr<Enemy>>& pEnemies);
	void Movement();
	void ShootBullet(const SDL_MouseButtonEvent& e, std::shared_ptr<Camera> camera);
	void Shooting(const SDL_MouseMotionEvent& e, std::shared_ptr<Camera> camera);
	void ResetPriority(const SDL_MouseButtonEvent& e);
	bool IsDead() const;
	Rectf GetShape() const;
	Point2f GetPosition() const;
	bool DoHitTest(const Rectf& other);
	void ReceiveDamage(int Damage);
	void ReceiveExperience(int Experience);
	void ReceiveHp(int hp);
	void ReceiveManna(int manna);
	Point2f GetHp() const;
	Point2f GetManna() const;
	Point2f GetXp() const;
	int GetLvl() const;
	int DoDamage();
	void ChangeVolume(int value);
private:
	RayCast m_RayCast;
	Point2f m_Position;
	Vector2f m_Speed;
	int m_Health;
	int m_Manna;
	int m_Defence;
	int m_Attack;
	Vector2f m_HorizontalVelocity;
	Vector2f m_VerticalVelocity;
	int m_Level;
	int m_Experience;
	int m_MaxExperience;
	float m_LevelConstant;
	std::unique_ptr<Texture> m_pAvatarTexture;
	AvatarOrientation m_AvatarOrientation;
	int m_Cols;
	int m_Rows;
	float m_FrameSec;
	float m_AccuSet;
	int m_Actframe;
	float m_AccuSet1;
	int m_Actframe1;
	int m_HealthMax;
	int m_MannaMax;
	float m_MouseAngle;
	bool m_PriorityMovement;
	bool m_Shooting;
	bool m_IsDead;
	Rectf m_Shape;
	std::deque<std::shared_ptr<AvatarBullet>> m_pBullets;
	std::deque<std::shared_ptr<AvatarBullet>> m_pSpecialAbility;
	std::unique_ptr<SoundEffect> m_pBulletSound;
	std::unique_ptr<SoundEffect> m_pSpecialSound;
	std::unique_ptr<SoundEffect> m_pLevelUpSound;
	std::unique_ptr<SoundEffect> m_pNoMannaSound;
	std::unique_ptr<SoundEffect> m_pHitSound;
	std::unique_ptr<SoundEffect> m_pDeathSound;
	int m_Volume;
	bool m_ShootDelay;
	bool m_SpecialDelay;
	float m_AccuSet2;
	float m_AccuSet3;
	float m_AccuSetManna;
	float m_AccuSetHealth;
	bool m_SpecialEnable;
	void SpecialAbility();
	void LvlUp();
	void RegenHealth(float elapsedSec);
	void RegenManna(float elapsedSec);
	void UpdateTexture(float elapsedSec);
	void DrawTexture() const;
	void DrawHealth() const;
	void CheckDead();
	void UpdateBullets(float elapsedSec, std::deque<std::shared_ptr<Enemy>>& pEnemies);
};

