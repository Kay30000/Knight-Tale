/// \file Player.cpp
/// \brief Code for the player object class CPlayer.

#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Object.h"
#include "Particle.h"
#include "ParticleEngine.h"


CPlayer::CPlayer(eSprite t, const Vector2& p) : CObject(t, p) {
	
	m_bIsTarget = true;

	m_fSpeed = 0.0f; 
	m_bStrafeLeft = false;
	m_bStrafeRight = false; 
	m_bStrafeBack = false; 
	m_bStatic = false;

	m_pFrameEvent = new LEventTimer(0.12f);
	
	m_fRadius = 16.0f; // player collision radius

	m_pBulletCooldown = new LEventTimer(COOLDOWN_BULLET);
	m_pFireballCooldown = new LEventTimer(COOLDOWN_FIREBALL);
	m_pSwordCooldown = new LEventTimer(COOLDOWN_SWORD);
	m_pGreatswordCooldown = new LEventTimer(COOLDOWN_GREATSWORD);
	m_pDaggerCooldown = new LEventTimer(COOLDOWN_DAGGER);

	m_bShieldActive = false;
	m_pShieldObject = nullptr;
} 

CPlayer::~CPlayer() {
	
	delete m_pFrameEvent;
	delete m_pBulletCooldown;
	delete m_pFireballCooldown;
	delete m_pSwordCooldown;
	delete m_pGreatswordCooldown;
	delete m_pDaggerCooldown;

} 

void CPlayer::move() {
	CObject::move();

	const float t = m_pTimer->GetFrameTime();
	const float delta = m_fSpeed * t;

	m_fRotSpeed = 0.0f;
	NormalizeAngle(m_fRoll);


	if (m_bStrafeBack || (m_fSpeed > 0.0f && !m_bStrafeRight && !m_bStrafeLeft)) {
		m_vPos.y += delta;
	}


	if (m_bStrafeRight || m_bStrafeLeft) {
		m_vPos.x += delta;
	}

	m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = false;


	if (m_bShieldActive && m_pShieldObject) {
		Vector2 playerDir = GetDirectionVector();
		m_pShieldObject->m_vPos = m_vPos + playerDir * SHIELD_OFFSET;
		m_pShieldObject->m_fRoll = m_fRoll;
	}

	UpdateFramenumber();
}



/*void CPlayer::move()
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
}*/
//Not sure which move() is the intended behavior so I just commented out the second one

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
/*
void CPlayer::WalkLeft() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkLeft) {
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkLeft;
		m_nCurrentFrame = 0;
	}
}
void CPlayer::WalkRight() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkRight) {
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkRight;
		m_nCurrentFrame = 0;
	}
}
void CPlayer::WalkUp() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkUp) {
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkUp;
		m_nCurrentFrame = 0;
	}
}
void CPlayer::WalkDown() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkDown) {
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkDown;
		m_nCurrentFrame = 0;
	}
}
*/
//not sure which set of movement functions is intended

/*
void CPlayer::Stop() {
	if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkRight)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandRight;

	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkLeft)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandLeft;

	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkUp)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandUp;

	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkDown)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandDown;

	m_nCurrentFrame = 0;
} */
//not sure which stop function is intended

void CPlayer::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
	if (m_bDead)return;

	if (pObj && pObj->isBullet()) {


		if (m_bShieldActive) {
			Vector2 playerDir = GetDirectionVector();
			Vector2 toBullet = Normalize(pObj->GetPos() - m_vPos);


			const float SHIELD_BLOCK_THRESHOLD = 0.5f;

			if (Dot(playerDir, toBullet) > SHIELD_BLOCK_THRESHOLD) {
				pObj->SetDead();
				m_pAudio->play(eSound::Clang);
				return;
			}
		}




		if (m_bGodMode)
			m_pAudio->play(eSound::Grunt);

		else if (--m_nHealth == 0) {

			m_pAudio->play(eSound::Boom);
			m_bDead = true;
			DeathFX();
			m_pPlayer = nullptr;
		}

		else {

			m_pAudio->play(eSound::Grunt);
			const float f = 0.5f + 0.5f * (float)m_nHealth / m_nMaxHealth;
			m_f4Tint = XMFLOAT4(1.0f, f, f, 0);
		}
	}

	CObject::CollisionResponse(norm, d, pObj);
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

/*void CPlayer::CollisionResponse(const Vector2& norm, float d, CObject* pObj)
{
    if (m_bDead) return;
    CObject::CollisionResponse(norm, d, pObj);
}*/ 
//Pretty sure this isn't the intended collision response, but I commented it out incase there was a change I wasn't aware of

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
