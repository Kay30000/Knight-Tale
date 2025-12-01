//Enemy.h
#pragma once
#include "Object.h"
#include "EventTimer.h"

class CEnemy : public CObject
{
public:
    CEnemy(const Vector2& pos);
    ~CEnemy();

    void Update(float dt) override;   // override OK

    void TakeDamage(int damage); // declare TakeDamage so other code can call it
    

private:
    float m_fHealth = 0.0f; // health for the enemy
    LEventTimer* m_pFrameEvent = nullptr;
    LEventTimer* m_pAttackCooldown = nullptr;
};
