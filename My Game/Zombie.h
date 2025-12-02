/// \file Zombie.h
/// \brief Interface for the Zombie object class CZombie.

#ifndef __L4RC_GAME_Zombie_H__ //__L4RC_GAME_TURRET_H__
#define __L4RC_GAME_Zombie_H__ //__L4RC_GAME_TURRET_H__

#include "Object.h"



class CZombie: public CObject{
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
    
    LEventTimer* m_pFrameEvent = nullptr;
    Vector2 m_vLastDirection = Vector2::Zero;



    Vector2 m_vHomePos;

  public:
    void InitializePatrol(const std::vector<Vector2>& points);
    CZombie(const Vector2& p); 
    ~CZombie();


    virtual void move(); 
    virtual void TakeDamage(int damage);
    void ZombieWalkLeft();
    void ZombieWalkRight();
    void ZombieWalkUp();
    void ZombieWalkDown();
    void ZombieStand();


    UINT m_nSpriteLeft = 0;
    UINT m_nSpriteRight = 0;
    UINT m_nSpriteUp = 0;
    UINT m_nSpriteDown = 0;

}; //CBullet

#endif //__L4RC_GAME_Zombie_H__
