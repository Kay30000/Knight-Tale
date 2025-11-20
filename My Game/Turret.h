/// \file Turret.h
/// \brief Interface for the turret object class CTurret.

#ifndef __L4RC_GAME_TURRET_H__
#define __L4RC_GAME_TURRET_H__

#include "Object.h"



class CTurret: public CObject{
  protected:
    const UINT m_nMaxHealth = 8; 
    UINT m_nHealth = m_nMaxHealth; 
    
    void RotateTowards(const Vector2&); ///< Swivel towards position.
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

  public:
    CTurret(const Vector2& p); 
    virtual void move(); 
    virtual void TakeDamage(int damage) ;

}; //CBullet

#endif //__L4RC_GAME_TURRET_H__
