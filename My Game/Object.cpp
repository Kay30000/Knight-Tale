/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"

/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
CObject::CObject(eSprite t, const Vector2& p)
    : LBaseObject(t, p)
{
    m_fRoll = XM_PIDIV2;
    m_bIsTarget = false;

    // Identify bullets
    if (t == eSprite::Bullet || t == eSprite::Fireball ||
        t == eSprite::sword || t == eSprite::greatsword || t == eSprite::dagger)
    {
        m_bIsBullet = true;
        m_bStatic = false;
    }
    else
    {
        m_bIsBullet = false;
    }

    // Compute collision radius
    float w = m_pRenderer->GetWidth(t);
    float h = m_pRenderer->GetHeight(t);
    m_fRadius = std::max(w, h) / 2.0f;

    m_pGunFireEvent = new LEventTimer(1.0f);
}

/// ---------------------------------------------------------------------------
/// Destructor
/// ---------------------------------------------------------------------------
CObject::~CObject()
{
    delete m_pGunFireEvent;
}

/// ---------------------------------------------------------------------------
/// Base movement
/// ---------------------------------------------------------------------------
void CObject::move()
{
    if (!m_bDead && !m_bStatic)
        m_vPos += m_vVelocity * m_pTimer->GetFrameTime();
}
void CObject::move(){
  if(!m_bDead && !m_bStatic)
    m_vPos += m_vVelocity*m_pTimer->GetFrameTime();

  if (m_bIsBullet && m_fMaxLifeSpan > 0.0f) {
      m_fTimeAlive += m_pTimer->GetFrameTime();

      if (m_fTimeAlive >= m_fMaxLifeSpan) {
          SetDead();
      }
  }

} //move

/// Ask the renderer to draw the sprite described in the sprite descriptor.
/// Note that `CObject` is derived from `LBaseObject` which is inherited from
/// `LSpriteDesc2D`. Therefore `LSpriteRenderer::Draw(const LSpriteDesc2D*)`
/// will accept `this` as a parameter, automatically down-casting it from
/// `CObject*` to `LSpriteDesc2D*`, effectively drawing the object from its
/// sprite descriptor.

void CObject::draw(){ 
  m_pRenderer->Draw(this);
} //draw

/// Response to collision. Move back the overlap distance along the collision
/// normal. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CObject::CollisionResponse(const Vector2& norm, float d, CObject* pObj){

  if(m_bDead)return;


  const Vector2 vOverlap = d*norm; 
  const bool bStatic = !pObj || pObj->m_bStatic; 

/// ---------------------------------------------------------------------------
/// Draw
/// ---------------------------------------------------------------------------
void CObject::draw()
{
    m_pRenderer->Draw(this);
}

/// ---------------------------------------------------------------------------
/// Collision response
/// ---------------------------------------------------------------------------
void CObject::CollisionResponse(const Vector2& norm, float d, CObject* pObj)
{
    if (m_bDead) return;

    Vector2 vOverlap = d * norm;
    bool collidingWithStatic = (!pObj || pObj->m_bStatic);

    if (!m_bStatic && !collidingWithStatic)
        m_vPos += vOverlap / 2.0f;
    else if (!m_bStatic)
        m_vPos += vOverlap;
}

/// ---------------------------------------------------------------------------
/// Death FX (stub)
/// ---------------------------------------------------------------------------
void CObject::DeathFX()
{
    // intentionally empty
}

/// ---------------------------------------------------------------------------
/// Set sprite index
/// ---------------------------------------------------------------------------
void CObject::SetSprite(eSprite t)
{
    m_nSpriteIndex = (UINT)t;
}

/// ---------------------------------------------------------------------------
/// Set animation frame (from tile furniture)
/// ---------------------------------------------------------------------------
void CObject::SetFrame(eSprite t, char c)
{
    m_nCurrentFrame = c - '0';
}

/// ---------------------------------------------------------------------------
/// View vector from roll angle
/// ---------------------------------------------------------------------------
const Vector2 CObject::GetViewVector() const
{
    return AngleToVector(m_fRoll);
}

/// ---------------------------------------------------------------------------
/// Is object a bullet?
/// ---------------------------------------------------------------------------
bool CObject::isBullet() const
{
    return m_bIsBullet;
}

