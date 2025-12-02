/// \file Turret.h
/// \brief Interface for the turret object class CTurret.

#ifndef __L4RC_GAME_TURRET_H__
#define __L4RC_GAME_TURRET_H__

#include "Object.h"



class CTurret : public CObject {
protected:
    const UINT m_nMaxHealth = 8;
    UINT m_nHealth = m_nMaxHealth;

    void RotateTowards(const Vector2&); ///< Swivel towards position.
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

    std::vector<Vector2> m_vPatrolPoints;
    int m_nCurrentPatrolIndex = 0;
    float m_fPatrolSpeed = 200.0f;

    bool m_bChasing = false;
    float m_fFollowRadius = 200.0f;
    float m_fReturnRadius = 500.0f;
    float m_fReturnSpeed = 2.0f;

    Vector2 m_vHomePos;

public:
    void InitializePatrol(const std::vector<Vector2>& points);
    CTurret(const Vector2& p);
    virtual void move();
    virtual void TakeDamage(int damage);

}; //CBullet

#endif //__L4RC_GAME_TURRET_H__