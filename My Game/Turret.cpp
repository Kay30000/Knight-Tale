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
#include "GameDefines.h"

/// Create and initialize a turret object given its position.
/// \param p Position of turret.

CTurret::CTurret(const Vector2& p): CObject(eSprite::Turret, p){
  m_bStatic = true; //turrets are static
  m_fPatrolSpeed = 70.0f;
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

void CTurret::move() {
    const float t = m_pTimer->GetFrameTime();
    Vector2 desiredDir(0, 0);
    float moveSpeed = 0.0f;
    Vector2 targetPos;

    if (m_pPlayer) {
        float distToPlayer = (m_pPlayer->GetPos() - m_vPos).Length();

        if (distToPlayer < m_fFollowRadius)
            m_bChasing = true;
        else if (distToPlayer > m_fReturnRadius)
            m_bChasing = false;
    }

    if (m_bChasing && m_pPlayer) {
        targetPos = m_pPlayer->GetPos();
        moveSpeed = m_fPatrolSpeed * 2.5f;
    }
    else {
        if (!m_vPatrolPoints.empty()) {
            targetPos = m_vPatrolPoints[m_nCurrentPatrolIndex];

            if ((targetPos - m_vPos).Length() < 5.0f) {
                m_nCurrentPatrolIndex = (m_nCurrentPatrolIndex + 1) % m_vPatrolPoints.size();
                targetPos = m_vPatrolPoints[m_nCurrentPatrolIndex];
            }
            moveSpeed = m_fPatrolSpeed;
        }
        else {
            targetPos = m_vHomePos;
            if ((targetPos - m_vPos).Length() < 5.0f) {
                moveSpeed = 0.0f;
            }
            else {
                moveSpeed = m_fReturnSpeed;
            }
        }
    }

    if (moveSpeed > 0.0f) {
        Vector2 toTarget = targetPos - m_vPos;
        float absX = fabsf(toTarget.x);
        float absY = fabsf(toTarget.y);

        if (absX > absY) {
            desiredDir.x = toTarget.x > 0 ? 1.0f : -1.0f;
            desiredDir.y = 0.0f;
        }
        else if (absY > absX) {
            desiredDir.x = 0.0f;
            desiredDir.y = toTarget.y > 0 ? 1.0f : -1.0f;
        }
        else if (absX > 0.0f) {
            desiredDir.x = toTarget.x > 0 ? 1.0f : -1.0f;
            desiredDir.y = 0.0f;
        }
    }

    if (vx > 0)
        SetSprite(ZombieWalkRightSpriteSheet);
    else if (vx < 0)
        SetSprite(ZombieWalkLeftSpriteSheet);
    else if (vy > 0)
        SetSprite(ZombieWalkDownSpriteSheet);
    else if (vy < 0)
        SetSprite(ZombieWalkUpSpriteSheet);


    Vector2 nextPos = m_vPos + desiredDir * moveSpeed * t;
    BoundingSphere s(Vector3(nextPos), m_fRadius);
    Vector2 norm;
    float d;

    if (m_pTileManager->CollideWithWall(s, norm, d)) {
        nextPos += norm * d;
    }

    m_vPos = nextPos;
    m_fRotSpeed = 0.0f;
}

void CTurret::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead)return;

    if (pObj && !pObj->isBullet()) {
        const float MAX_PUSH_DISTANCE = 5.0f;
        const float COLLISION_EPSILON = 0.05f;
        float required_total_push = d + COLLISION_EPSILON;
        float required_push_half = required_total_push * 0.5f;
        float safe_d = std::min(required_push_half, MAX_PUSH_DISTANCE);
        m_vPos += norm * safe_d;
    }

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