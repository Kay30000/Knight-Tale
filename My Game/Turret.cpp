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
  m_fPatrolSpeed = 20.0f;
  m_vHomePos = p; // store spawn position
} //constructor

void CTurret::InitializePatrol(const std::vector<Vector2>& points) {
    m_vPatrolPoints = points;
    m_nCurrentPatrolIndex = 0;
    if (!points.empty()) {
        m_vPos = points[0];
        m_vHomePos = points[0];
    }
}

void CTurret::move(){
    const float t = m_pTimer->GetFrameTime();
    Vector2 desiredDir(0, 0);

    if (m_pPlayer) {
        float distToPlayer = (m_pPlayer->GetPos() - m_vPos).Length();

        // If player is close enough, start chasing
        if (distToPlayer < m_fFollowRadius)
            m_bChasing = true;
        // If player is far away, stop chasing
        else if (distToPlayer > m_fReturnRadius)
            m_bChasing = false;
    }

    if (m_bChasing && m_pPlayer) {
        // Move toward the player
        Vector2 toPlayer = Normalize(m_pPlayer->GetPos() - m_vPos);
        m_vPos += toPlayer * m_fPatrolSpeed * 2.5f * t; // slightly faster chase
    }
    else {
        // Return to home or patrol route
        if (!m_vPatrolPoints.empty()) {
            // Use your existing patrol logic
            Vector2 target = m_vPatrolPoints[m_nCurrentPatrolIndex];
            Vector2 toTarget = target - m_vPos;

            if (toTarget.Length() < 5.0f) {
                m_nCurrentPatrolIndex = (m_nCurrentPatrolIndex + 1) % m_vPatrolPoints.size();
            }
            else {
                Vector2 dir = Normalize(toTarget);
                m_vPos += dir * m_fPatrolSpeed * t;
            }
        }
        else {
            // No patrol path — just go back to home
            Vector2 toHome = m_vHomePos - m_vPos;
            if (toHome.Length() > 5.0f) {
                Vector2 dir = Normalize(toHome);
                m_vPos += dir * m_fPatrolSpeed * t;
            }
        }
    }

    Vector2 nextPos = m_vPos + desiredDir * t;

    BoundingSphere s(Vector3(nextPos), m_fRadius);
    Vector2 norm;
    float d = 0.0f;

    // Push turret out of walls if overlap occurs
    if (m_pTileManager->CollideWithWall(s, norm, d)) {
        nextPos += norm * d;
    }

    // Apply safe movement
    m_vPos = nextPos;

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
        else if (pObj->m_nSpriteIndex == (UINT)eSprite::sword) {
            damageToTake = SWORD_DAMAGE;
        }
        else if (pObj->m_nSpriteIndex == (UINT)eSprite::greatsword) {
            damageToTake = GREATSWORD_DAMAGE;
        }

        else if (pObj->m_nSpriteIndex == (UINT)eSprite::dagger) {
            damageToTake = DAGGER_DAMAGE;
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