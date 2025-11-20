/// \file Player.cpp
/// \brief Code for the player object class CPlayer.

#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Object.h"
#include "Particle.h"
#include "ParticleEngine.h"

CPlayer::CPlayer(eSprite t, const Vector2& p) :
    CObject(t, p)
{
    m_bIsTarget = true;
    m_bStatic = false;

    m_fSpeed = 0.0f;
    m_bStrafeLeft = false;
    m_bStrafeRight = false;
    m_bStrafeBack = false;

    m_pFrameEvent = new LEventTimer(0.12f);

    m_fRadius = 16.0f;

    m_pBulletCooldown = new LEventTimer(COOLDOWN_BULLET);
    m_pFireballCooldown = new LEventTimer(COOLDOWN_FIREBALL);
    m_pSwordCooldown = new LEventTimer(COOLDOWN_SWORD);
    m_pGreatswordCooldown = new LEventTimer(COOLDOWN_GREATSWORD);
    m_pDaggerCooldown = new LEventTimer(COOLDOWN_DAGGER);
}

CPlayer::~CPlayer()
{
    delete m_pFrameEvent;
    delete m_pBulletCooldown;
    delete m_pFireballCooldown;
    delete m_pSwordCooldown;
    delete m_pGreatswordCooldown;
    delete m_pDaggerCooldown;
}

void CPlayer::move()
{
    const float t = m_pTimer->GetFrameTime();
    const float baseSpeed = 300.0f;


    m_vVelocity = Vector2(0, 0);

    if (m_fSpeed > 0.0f)
        m_vVelocity.y += baseSpeed;
    if (m_bStrafeBack)
        m_vVelocity.y -= baseSpeed;
    if (m_bStrafeRight)
        m_vVelocity.x += baseSpeed;
    if (m_bStrafeLeft)
        m_vVelocity.x -= baseSpeed;

    m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = false;

    if (m_vVelocity.Length() > 0.1f)
    {
        m_vVelocity.Normalize();
        m_vVelocity *= baseSpeed;
    }

    m_vPos += m_vVelocity * t;

    if (m_vVelocity.Length() > 0.1f)
    {
        if (fabsf(m_vVelocity.x) > fabsf(m_vVelocity.y))
        {
            if (m_vVelocity.x > 0) WalkRight();
            else WalkLeft();
        }
        else
        {
            if (m_vVelocity.y > 0) WalkUp();
            else WalkDown();
        }
    }
    else
        Stop();
}

void CPlayer::TriggerAttack()
{
    if (m_bAttacking) return;

    m_bAttacking = true;
    m_fAttackTimer = 0.0f;

    if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkRight || m_nSpriteIndex == (UINT)eSprite::PlayerStandRight) {
        SetSprite(eSprite::PlayerAttackRight);
    }
    else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkLeft || m_nSpriteIndex == (UINT)eSprite::PlayerStandLeft) {
        SetSprite(eSprite::PlayerAttackLeft);
    }
    else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkUp || m_nSpriteIndex == (UINT)eSprite::PlayerStandUp) {
        SetSprite(eSprite::PlayerAttackUp);
    }
    else {
        SetSprite(eSprite::PlayerAttackDown);
    }
}

void CPlayer::Update(float dt)
{
    if (m_bAttacking) {
        m_fAttackTimer += dt;
        if (m_fAttackTimer >= m_fAttackDuration) {
            m_bAttacking = false;
            
            Stop();                 // Go back to idle sprite
        }
        return;
    }

    UpdateFramenumber();
}


void CPlayer::UpdateFramenumber()
{
    if (!m_pRenderer || m_nSpriteIndex >= (UINT)eSprite::Size)
        return;

    UINT n = m_pRenderer->GetNumFrames(m_nSpriteIndex);

    if (n > 1 && m_pFrameEvent && m_pFrameEvent->Triggered()) {
        m_nCurrentFrame++;
        if (!m_bAttacking)
            m_nCurrentFrame %= n;
        else if (m_nCurrentFrame >= n)
            m_nCurrentFrame = n - 1;  // stop on final attack frame
    }
}

// ------------ Animation Helpers ------------

void CPlayer::SetSprite(eSprite sprite)
{
    // Clear rotation ONLY for non-attack sprites
    switch (sprite)
    {
    case eSprite::PlayerWalkUp:
    case eSprite::PlayerWalkDown:
    case eSprite::PlayerWalkLeft:
    case eSprite::PlayerWalkRight:
    case eSprite::PlayerStandUp:
    case eSprite::PlayerStandDown:
    case eSprite::PlayerStandLeft:
    case eSprite::PlayerStandRight:
        break;
    default:
        break; // Leave m_fRoll as-is for attack sprites
    }

    if (m_nSpriteIndex != (UINT)sprite) {
        m_nSpriteIndex = (UINT)sprite;
        m_nCurrentFrame = 0;
    }
    else {
        UINT n = m_pRenderer->GetNumFrames(m_nSpriteIndex);
        if (m_nCurrentFrame >= n)
            m_nCurrentFrame = 0;
    }
}





void CPlayer::WalkLeft() { SetSprite(eSprite::PlayerWalkLeft); }
void CPlayer::WalkRight() { SetSprite(eSprite::PlayerWalkRight); }
void CPlayer::WalkUp() { SetSprite(eSprite::PlayerWalkUp); }
void CPlayer::WalkDown() { SetSprite(eSprite::PlayerWalkDown); }

void CPlayer::Stop()
{

    if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkRight)
        SetSprite(eSprite::PlayerStandRight);
    else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkLeft)
        SetSprite(eSprite::PlayerStandLeft);
    else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkUp)
        SetSprite(eSprite::PlayerStandUp);
    else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkDown)
        SetSprite(eSprite::PlayerStandDown);
}




// ------------ Collision ------------

void CPlayer::CollisionResponse(const Vector2& norm, float d, CObject* pObj)
{
    if (m_bDead) return;
    CObject::CollisionResponse(norm, d, pObj);
}

// ------------ Movement Flags ------------

void CPlayer::StrafeLeft() { m_bStrafeLeft = true; }
void CPlayer::StrafeRight() { m_bStrafeRight = true; }
void CPlayer::StrafeBack() { m_bStrafeBack = true; }
void CPlayer::SetSpeed(float s) { m_fSpeed = s; }
void CPlayer::SetRotSpeed(float s) { m_fRotSpeed = s; }

// ------------ Accessors ------------

const Vector2& CPlayer::GetPos() const { return m_vPos; }

Vector2 CPlayer::GetDirectionVector()
{
    if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkRight ||
        m_nSpriteIndex == (UINT)eSprite::PlayerStandRight)
        return Vector2(1, 0);
    if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkLeft ||
        m_nSpriteIndex == (UINT)eSprite::PlayerStandLeft)
        return Vector2(-1, 0);
    if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkUp ||
        m_nSpriteIndex == (UINT)eSprite::PlayerStandUp)
        return Vector2(0, 1);

    return Vector2(0, -1); // down default
}

void CPlayer::DeathFX()
{
    LParticleDesc2D d;
    d.m_vPos = m_vPos;
    d.m_nSpriteIndex = (UINT)eSprite::Smoke;
    m_pParticleEngine->create(d);
}

// ------------ Damage ------------

void CPlayer::TakeDamage(int damage)
{
    if (m_bDead) return;
    if (damage <= 0) return;

    if (damage >= (int)m_nHealth) {
        m_nHealth = 0;
        m_bDead = true;
        DeathFX();
    }
    else {
        m_nHealth -= (UINT)damage;
    }

    // small hit effect
    LParticleDesc2D pd;
    pd.m_vPos = m_vPos;
    pd.m_nSpriteIndex = (UINT)eSprite::Smoke;
    m_pParticleEngine->create(pd);
}
