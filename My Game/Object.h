/// \file Object.h

#ifndef __L4RC_GAME_OBJECT_H__
#define __L4RC_GAME_OBJECT_H__

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"
#include "BaseObject.h"
#include "EventTimer.h"

class CObject :
    public CCommon,
    public LBaseObject
{
    friend class CObjectManager;

protected:

    float m_fRadius = 0.0f;
    float m_fSpeed = 0.0f;
    float m_fRotSpeed = 0.0f;

    /*
    Vector2 m_vVelocity = Vector2(0, 0);
    */
    bool m_bStatic = true;
    bool m_bIsTarget = true;
    bool m_bIsBullet = false;
    
    Vector2 m_vVelocity; 
    
	

    float m_fMaxLifeSpan = 0.0f;
    float m_fTimeAlive = 0.0f;

    LEventTimer* m_pGunFireEvent = nullptr;

    virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr);
    virtual void DeathFX();

public:

    float resistance = 1; //Resistance to movement depending on tile effects
    
    bool isHealthBar = false;
    bool isFurniture = false; ///< Is furniture
    CObject(eSprite, const Vector2&); ///< Constructor.
    virtual ~CObject(); ///< Destructor.

    virtual void Update(float dt) {}

    void move();
    void draw();

    void SetSprite(eSprite t);
    void SetFrame(eSprite t, char c);

    // DECLARATIONS ONLY (no inline code!)
    bool isBullet() const;
    const Vector2 GetViewVector() const;

    void SetDead() { m_bDead = true; }


    const Vector2& GetPos() const { return m_vPos; }
    void SetStatic(bool isStatic) { m_bStatic = isStatic; }
}; 

#endif
