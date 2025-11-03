/// \file Object.cpp
/// \brief Code for the game object class CObject.

#include "Object.h"
#include "ComponentIncludes.h"
#include "GameDefines.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"

/// Create and initialize an object given its sprite type and initial position.
/// \param t Type of sprite.
/// \param p Initial position of object.

CObject::CObject(eSprite t, const Vector2& p):
  LBaseObject(t, p)
{ 
  m_fRoll = XM_PIDIV2; 
  m_bIsTarget = false;


  if (t == eSprite::Bullet || t == eSprite::Fireball) {
      m_bIsBullet = true;
      m_bStatic = false;
  }
  else {
      m_bIsBullet = false;
  }

  const float w = m_pRenderer->GetWidth(t); 
  const float h = m_pRenderer->GetHeight(t);
  m_fRadius = std::max(w, h)/2; 

  m_pGunFireEvent = new LEventTimer(1.0f); 
} 


CObject::~CObject(){
  delete m_pGunFireEvent;
}


void CObject::move(){
  if(!m_bDead && !m_bStatic)
    m_vPos += m_vVelocity*m_pTimer->GetFrameTime();
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

  if(!m_bStatic && !bStatic) 
    m_vPos += vOverlap/2; 

  else if(!m_bStatic && bStatic) 
    m_vPos += vOverlap; 
} 

/// Create a particle effect to mark the death of the object.
/// This function is a stub intended to be overridden by various object classes
/// derived from this class.

void CObject::DeathFX(){
 //stub
} //DeathFX


void CObject::SetSprite(eSprite t) {
    m_nSpriteIndex = (UINT)t;
}

void CObject::SetFrame(eSprite t, char c) {
    
    m_nCurrentFrame = c - '0';
}

/// Compute the view vector from the object orientation.
/// \return The view vector.


const Vector2 CObject::GetViewVector() const{
  return AngleToVector(m_fRoll);
} 

const bool CObject::isBullet() const{
  return m_bIsBullet;
} 