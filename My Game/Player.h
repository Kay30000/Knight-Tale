/// \file Player.h
/// \brief Interface for the player object class CPlayer.

#ifndef __L4RC_GAME_PLAYER_H__
#define __L4RC_GAME_PLAYER_H__

#include "Object.h"

/// \brief The player object. 
///
/// The abstract representation of the player object. The player differs from
/// the other objects in the game in that it moves in respond to device inputs.

class CPlayer: public CObject{
  protected:  
    const UINT m_nMaxHealth = 12; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.

    bool m_bStrafeLeft = false; ///< Strafe left.
    bool m_bStrafeRight = false; ///< Strafe right.
    bool m_bStrafeBack = false; ///< Strafe back.
    
    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
    virtual void DeathFX(); ///< Death special effects.

  public:
    CPlayer(const Vector2& p); ///< Constructor.

    virtual void move(); ///< Move player object.

    void SetSpeed(const float speed); ///< Set speed.
    void SetRotSpeed(const float speed); ///< Set rotational velocity.
    
    void StrafeLeft(); ///< Strafe left.
    void StrafeRight(); ///< Strafe right.
    void StrafeBack(); ///< Strafe back.
    
    const Vector2& GetPos() const; ///< Get position.
}; //CPlayer

#endif //__L4RC_GAME_PLAYER_H__
