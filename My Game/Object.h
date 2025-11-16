/// \file Object.h
/// \brief Interface for the game object class CObject.

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"
#include "BaseObject.h"
#include "EventTimer.h"

/// \brief The game object. 
///
/// The abstract representation of an object. `CObjectManager` is a friend of
/// this class so that it can access any private members as needed to manage
/// the objects without the need for reader and set functions for each private
/// or protected member variable. This class must contain public member
/// functions `move()` and `draw()` to move and draw the object, respectively.

class CObject:
  public CCommon,
  public LBaseObject
{
  friend class CObjectManager; 

  protected:
    float m_fRadius = 0;


    float m_fSpeed = 0; 
    float m_fRotSpeed = 0;
    Vector2 m_vVelocity; 
    bool m_bStatic = true; 
    bool m_bIsTarget = true; 
    bool m_bIsBullet = false; 
	

    float m_fMaxLifeSpan = 0.0f;
    float m_fTimeAlive = 0.0f;


    LEventTimer* m_pGunFireEvent = nullptr; 
    
    virtual void CollisionResponse(const Vector2&, float,
      CObject* = nullptr);
    virtual void DeathFX(); 

    const Vector2 GetViewVector() const;

  public:

    bool isHealthBar = false;
    bool isFurniture = false; ///< Is furniture
    CObject(eSprite, const Vector2&); ///< Constructor.
    virtual ~CObject(); ///< Destructor.


    void move();
    void draw();


    void SetSprite(eSprite t);
    void SetFrame(eSprite t, char c);

    const bool isBullet() const; 


    void SetDead() { m_bDead = true; }
}; 

#endif 