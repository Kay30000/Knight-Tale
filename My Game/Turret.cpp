/// \file Turret.cpp
/// \brief Code for the turret object class CTurret.

#include "Turret.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize a turret object given its position.
/// \param p Position of turret.

CTurret::CTurret(const Vector2& p): CObject(eSprite::Turret, p){
  m_bStatic = true; //turrets are static
} //constructor

void CTurret::move(){
  if(m_pPlayer){ //safety
    const float r = ((CTurret*)m_pPlayer)->m_fRadius;

    if(m_pTileManager->Visible(m_vPos, m_pPlayer->m_vPos, r)) 
      RotateTowards(m_pPlayer->m_vPos);
    
  } 

  m_fRoll += 0.2f*m_fRotSpeed*XM_2PI*m_pTimer->GetFrameTime(); 
  NormalizeAngle(m_fRoll); 
} 

void CTurret::RotateTowards(const Vector2& pos){
  const Vector2 v = pos - m_vPos; //vector from target to turret
  const float theta = atan2f(v.y, v.x); //orientation of that vector
  float diff = m_fRoll - theta; //difference with turret orientation
  NormalizeAngle(diff); //normalize to [-pi, pi]

  //set rotation speed from diff

  const float fAngleDelta = 0.05f; //allowable angle discrepancy
  const float fTrackingSpeed = 2.0f; //rotation speed when tracking

  if(diff > fAngleDelta)m_fRotSpeed = -fTrackingSpeed; //clockwise
  else if(diff < -fAngleDelta)m_fRotSpeed = fTrackingSpeed; //counterclockwise
  else m_fRotSpeed = 0; //stop rotating

  
} 

/// Response to collision. 
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CTurret::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return;

    if (pObj && pObj->isBullet()) {

        int damageToTake = 1;

        if (pObj->m_nSpriteIndex == (UINT)eSprite::Fireball) {
            damageToTake = FIREBALL_DAMAGE;
        }
        pObj->SetDead();

        this->TakeDamage(damageToTake);
    }
}



void CTurret::TakeDamage(int damage) {
    if (m_bDead) return;
  
    if (m_nHealth > (UINT)damage) {
        m_nHealth -= damage;
    }
    else {
        m_nHealth = 0;
    }

    if (m_nHealth <= 0) {
        m_pAudio->play(eSound::Boom);
        m_bDead = true;
        DeathFX();
    }
    else {
        m_pAudio->play(eSound::Clang);
        const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth;
        m_f4Tint = XMFLOAT4(1.0f, f, f, 0); 
    }
}

void CTurret::DeathFX() {
    LParticleDesc2D d;
    d.m_vPos = m_vPos;

    d.m_nSpriteIndex = (UINT)eSprite::Smoke;
    d.m_fLifeSpan = 2.0f;
    d.m_fMaxScale = 4.0f;
    d.m_fScaleInFrac = 0.5f;
    d.m_fFadeOutFrac = 0.8f;
    d.m_fScaleOutFrac = 0;
    m_pParticleEngine->create(d);

    d.m_nSpriteIndex = (UINT)eSprite::Spark;
    d.m_fLifeSpan = 0.5f;
    d.m_fMaxScale = 1.5f;
    d.m_fScaleInFrac = 0.4f;
    d.m_fScaleOutFrac = 0.3f;
    d.m_fFadeOutFrac = 0.5f;
    d.m_f4Tint = XMFLOAT4(Colors::Orange);
    m_pParticleEngine->create(d);
} 