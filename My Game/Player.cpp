/// \file Player.cpp
/// \brief Code for the player object class CPlayer.

#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CPlayer::CPlayer(eSprite t, const Vector2& p) : CObject(t, p) {
	

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
	delete m_pFrameEvent;
	delete m_pBulletCooldown;
	delete m_pFireballCooldown;
	delete m_pSwordCooldown;
	delete m_pGreatswordCooldown;
	delete m_pDaggerCooldown;
	delete m_pSwordCooldown;

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

void CPlayer::UpdateFramenumber() {
	const UINT n = (UINT)m_pRenderer->GetNumFrames(m_nSpriteIndex);

	if (n > 1 && m_pFrameEvent && m_pFrameEvent->Triggered())
		m_nCurrentFrame = (m_nCurrentFrame + 1) % n;
}

void CPlayer::WalkLeft() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkLeft)
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkLeft;
	m_nCurrentFrame = 0;
} 
 
void CPlayer::WalkRight() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkRight)
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkRight;
	m_nCurrentFrame = 0;
} 

void CPlayer::WalkUp() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkUp)
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkUp;
	m_nCurrentFrame = 0;
} 

void CPlayer::WalkDown() {
	if (m_nSpriteIndex != (UINT)eSprite::PlayerWalkDown)
		m_nSpriteIndex = (UINT)eSprite::PlayerWalkDown;
	m_nCurrentFrame = 0;
} 

void CPlayer::Stop() {
	if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkRight)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandRight;

	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkLeft)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandLeft;

	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkUp)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandUp;

	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkDown)
		m_nSpriteIndex = (UINT)eSprite::PlayerStandDown;
} 

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

void CPlayer::DeathFX() {
	LParticleDesc2D d;
	d.m_vPos = m_vPos;

	d.m_nSpriteIndex = (UINT)eSprite::Smoke;
	
	m_pParticleEngine->create(d);
} 

void CPlayer::StrafeLeft() {
	m_bStrafeLeft = true; 
}

void CPlayer::StrafeRight() { 
	m_bStrafeRight = true; 
}

void CPlayer::StrafeBack() { 
	m_bStrafeBack = true; 
}

void CPlayer::SetSpeed(const float speed) { 
	m_fSpeed = speed; 
}

void CPlayer::SetRotSpeed(const float speed){
	m_fRotSpeed = speed;
}

const Vector2& CPlayer::GetPos() const{
	return m_vPos;
}


Vector2 CPlayer::GetDirectionVector() {
	Vector2 vDirection = Vector2(0.0f, -1.0f);

	if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkUp) {
		vDirection = Vector2(0.0f, 1.0f); // Up 
	}
	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkDown) {
		vDirection = Vector2(0.0f, -1.0f); // Down
	}
	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkRight) {
		vDirection = Vector2(1.0f, 0.0f); // Right 
	}
	else if (m_nSpriteIndex == (UINT)eSprite::PlayerWalkLeft) {
		vDirection = Vector2(-1.0f, 0.0f); // Left
	}

	else if (m_nSpriteIndex == (UINT)eSprite::PlayerStandUp) {
		vDirection = Vector2(0.0f, 1.0f);
	}
	else if (m_nSpriteIndex == (UINT)eSprite::PlayerStandDown) {
		vDirection = Vector2(0.0f, -1.0f);
	}
	else if (m_nSpriteIndex == (UINT)eSprite::PlayerStandRight) {
		vDirection = Vector2(1.0f, 0.0f);
	}
	else if (m_nSpriteIndex == (UINT)eSprite::PlayerStandLeft) {
		vDirection = Vector2(-1.0f, 0.0f);
	}

	return vDirection;
}
