/// \file Zombie.cpp
/// \brief Code for the Zombie object class CZombie.

#include "Zombie.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "GameDefines.h"

/// Create and initialize a Zombie object given its position.
/// \param p Position of Zombie.
CZombie::CZombie(const Vector2& p) : CObject(eSprite::ZombieStandDown, p) {
    m_bStatic = true;
    m_fPatrolSpeed = 70.0f;
    m_vHomePos = p;

    m_fRadius = 16.0f; // reduce zombie collision radius to match player/turret

    m_pFrameEvent = new LEventTimer(0.12f);
    m_pDirectionCooldown = new LEventTimer(0.1f); // add cooldown for direction flip

    m_nLastSpriteIndex = m_nSpriteIndex;
    m_vLastDirection = Vector2(0, -1); // default facing down
}

void CZombie::InitializePatrol(const std::vector<Vector2>& points) {
    m_vPatrolPoints = points;
    m_nCurrentPatrolIndex = 0;
    if (!points.empty()) {
        m_vPos = points[0];
        m_vHomePos = points[0];
    }
}

void CZombie::move() {
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
        desiredDir = GetDominantDirection(toTarget);
    }

    bool isMoving = desiredDir.x != 0 || desiredDir.y != 0;

    if (isMoving) {
        if (desiredDir != m_vLastDirection && m_pDirectionCooldown->Triggered()) {
            if (desiredDir.x > 0)
                m_nSpriteIndex = (UINT)eSprite::ZombieWalkRight;
            else if (desiredDir.x < 0)
                m_nSpriteIndex = (UINT)eSprite::ZombieWalkLeft;
            else if (desiredDir.y < 0)
                m_nSpriteIndex = (UINT)eSprite::ZombieWalkDown;
            else if (desiredDir.y > 0)
                m_nSpriteIndex = (UINT)eSprite::ZombieWalkUp;
            m_vLastDirection = desiredDir;
        }
    }
    else {
        switch (m_nSpriteIndex) {
        case (UINT)eSprite::ZombieWalkLeft:  m_nSpriteIndex = (UINT)eSprite::ZombieStandLeft; break;
        case (UINT)eSprite::ZombieWalkRight: m_nSpriteIndex = (UINT)eSprite::ZombieStandRight; break;
        case (UINT)eSprite::ZombieWalkUp:    m_nSpriteIndex = (UINT)eSprite::ZombieStandUp; break;
        case (UINT)eSprite::ZombieWalkDown:  m_nSpriteIndex = (UINT)eSprite::ZombieStandDown; break;
        default: break;
        }
    }

    if (m_nSpriteIndex != m_nLastSpriteIndex) {
        m_nCurrentFrame = 0;
        m_nLastSpriteIndex = m_nSpriteIndex;
    }

    m_bIsMoving = isMoving;

    Vector2 nextPos = m_vPos + desiredDir * moveSpeed * t;
    BoundingSphere s(Vector3(nextPos), m_fRadius);
    Vector2 norm;
    float d;

    if (m_pTileManager->CollideWithWall(s, norm, d)) {
        nextPos += norm * d;
    }

    m_vPos = nextPos;
    UpdateFramenumber();
    m_fRotSpeed = 0.0f;
}

void CZombie::UpdateFramenumber() {
    const UINT n = (UINT)m_pRenderer->GetNumFrames(m_nSpriteIndex);
    if (n > 1 && m_pFrameEvent && m_pFrameEvent->Triggered())
        m_nCurrentFrame = (m_nCurrentFrame + 1) % n;
}

Vector2 CZombie::GetDominantDirection(const Vector2& v) {
    return (fabsf(v.x) > fabsf(v.y)) ? Vector2((v.x > 0) ? 1 : -1, 0) : Vector2(0, (v.y > 0) ? 1 : -1);
}

void CZombie::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
    if (m_bDead) return;

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

        if (pObj->m_nSpriteIndex == (UINT)eSprite::Fireball) damageToTake = FIREBALL_DAMAGE;
        else if (pObj->m_nSpriteIndex == (UINT)eSprite::sword) damageToTake = SWORD_DAMAGE;
        else if (pObj->m_nSpriteIndex == (UINT)eSprite::greatsword) damageToTake = GREATSWORD_DAMAGE;
        else if (pObj->m_nSpriteIndex == (UINT)eSprite::dagger) damageToTake = DAGGER_DAMAGE;

        pObj->SetDead();
        TakeDamage(damageToTake);
    }
}

void CZombie::TakeDamage(int damage) {
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

void CZombie::DeathFX() {
    LParticleDesc2D d;
    d.m_vPos = m_vPos;

    d.m_nSpriteIndex = (UINT)eSprite::Smoke;
    d.m_fLifeSpan = 2.0f;
    d.m_fMaxScale = 4.0f;
    d.m_fScaleInFrac = 0.5f;
    d.m_fFadeOutFrac = 0.8f;
    m_pParticleEngine->create(d);

    d.m_nSpriteIndex = (UINT)eSprite::Spark;
    d.m_fLifeSpan = 0.5f;
    d.m_fMaxScale = 1.5f;
    d.m_fScaleInFrac = 0.4f;
    d.m_fFadeOutFrac = 0.5f;
    d.m_f4Tint = XMFLOAT4(Colors::Orange);
    m_pParticleEngine->create(d);
}

CZombie::~CZombie() {
    delete m_pFrameEvent;
    delete m_pDirectionCooldown;
}