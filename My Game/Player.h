/// \file Player.h
/// \brief Interface for the player object class CPlayer.

#ifndef __L4RC_GAME_PLAYER_H__
#define __L4RC_GAME_PLAYER_H__

#include "Object.h"
#include "EventTimer.h"
#include <iostream>


class CPlayer : public CObject {
    friend class CGame;

protected:
    const UINT m_nMaxHealth = 12;
    UINT m_nHealth = m_nMaxHealth;

    bool m_bStrafeLeft = false;
    bool m_bStrafeRight = false;
    bool m_bStrafeBack = false;

    LEventTimer* m_pFrameEvent = nullptr;
    LEventTimer* m_pBulletCooldown = nullptr;
    LEventTimer* m_pFireballCooldown = nullptr;
    LEventTimer* m_pSwordCooldown = nullptr;
    LEventTimer* m_pGreatswordCooldown = nullptr;
    LEventTimer* m_pDaggerCooldown = nullptr;



    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr);
    virtual void DeathFX();

    void UpdateFramenumber(); 

public:
    CPlayer(eSprite t, const Vector2& p);
    virtual ~CPlayer(); 

    virtual void move(); 

	void TakeDamage() //This is here for testing purposes, it might come in handy later so I'm leaving it
    {
		m_nHealth -= 1;
    }//Take Damage

    int GetHealth()
    {
		return m_nHealth;
    }

    int GetMaxHealth()
    {
        return m_nMaxHealth;
    }

    void WalkLeft();
    void WalkRight();
    void WalkUp();
    void WalkDown();
    void Stop(); 

    void SetSpeed(const float speed);
    void SetRotSpeed(const float speed);

    void StrafeLeft();
    void StrafeRight();
    void StrafeBack();

    const Vector2& GetPos() const;

    Vector2 GetDirectionVector();
}; 

#endif 