/// \file Player.cpp
/// \brief Code for the player object class CPlayer.

#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

CPlayer::CPlayer(eSprite t, const Vector2& p) : CObject(t, p) {
	m_bIsTarget = true;
	m_bStatic = false;

	m_fSpeed = 0.0f; 
	m_bStrafeLeft = false;
	m_bStrafeRight = false; 
	m_bStrafeBack = false; 

	m_pFrameEvent = new LEventTimer(0.12f);
} 

CPlayer::~CPlayer() {
	delete m_pFrameEvent; 
} 

void CPlayer::move() {
	const float t = m_pTimer->GetFrameTime(); 
	const float baseSpeed = 200.0f;
	const float delta = baseSpeed * t;

	m_fRotSpeed = 0.0f;
	NormalizeAngle(m_fRoll);

	// W Forward/Up movement 
	if (m_fSpeed > 0.0f) {
		m_vPos.y += delta;
	}

	// S Backward/Down movement 
	else if (m_bStrafeBack) {
		m_vPos.y -= delta;
	}

	// D Right movement
	if (m_bStrafeRight) {
		m_vPos.x += delta;
	}

	// A Left movement
	else if (m_bStrafeLeft) {
		m_vPos.x -= delta;
	}

	m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = false;
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