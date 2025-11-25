//Enemy.h
#pragma once
#include "Object.h"
#include "EventTimer.h"

class CEnemy : public CObject
{
public:
    // Change constructor to take Vector2, matching usage in ObjectManager.cpp
    CEnemy(const Vector2& pos);
    ~CEnemy();

    void Update(float dt);
    void Initialize();

    void TakeDamage(int damage);

private:
    float m_fHealth = 0.0f;
    LEventTimer* m_pFrameEvent = nullptr;
    LEventTimer* m_pAttackCooldown = nullptr;
};