/// \file Player.h
/// \brief Interface for the player object class CPlayer.

#ifndef __L4RC_GAME_PLAYER_H__
#define __L4RC_GAME_PLAYER_H__

#include "Object.h"
#include "EventTimer.h"

class CPlayer : public CObject {
    friend class CGame;

protected:
    const UINT m_nMaxHealth = 12;
    UINT m_nHealth = m_nMaxHealth;

    bool m_bStrafeLeft = false;
    bool m_bStrafeRight = false;
    bool m_bStrafeBack = false;

    bool m_bAttacking = false;
    float m_fAttackDuration = 0.3f; // duration of attack animation
    float m_fAttackTimer = 0.0f;

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

    void WalkLeft();
    void WalkRight();
    void WalkUp();
    void WalkDown();
    void Update(float dt) override;
    float GetRadius() const { return m_fRadius; }
    void TriggerAttack();

    void Stop(); 

    void SetSpeed(const float speed);
    void SetRotSpeed(const float speed);

    void StrafeLeft();
    void StrafeRight();
    void StrafeBack();

    const Vector2& GetPos() const;

    Vector2 GetDirectionVector();

    void TakeDamage(int damage); // added: allow others to damage player

private:
	void SetSprite(eSprite sprite);
}; 

#endif 