/// \file Enemy.cpp 
#include "ComponentIncludes.h" 
#include "TileManager.h" 
#include "GameDefines.h" 
#include "Enemy.h" 
#include "ObjectManager.h" 
#include "Player.h" 
#include "Helpers.h" 
#include "Particle.h" 
#include "ParticleEngine.h" 
#include "Object.h"

CEnemy::~CEnemy() {
	delete m_pFrameEvent;
	delete m_pAttackCooldown;
}

CEnemy::CEnemy(const Vector2& pos) :
	CObject(eSprite::SkeletonStandDown, pos) {
	m_pFrameEvent = new LEventTimer(0.12f);
	m_pAttackCooldown = new LEventTimer(2.0f);
	m_fSpeed = 80.0f; // enemy movement speed 
	m_fRadius = 16.0f;


	Initialize(); // <--- Call here to ensure it's ready
}

void CEnemy::Initialize() {
	m_pObjectManager = (CObjectManager*)CObject::m_pObjectManager;
	m_pTileManager = CObject::m_pTileManager;
	m_pRenderer = CObject::m_pRenderer;
}

void CEnemy::move() {
	float dt = m_pTimer->GetFrameTime();
	Update(dt);  // calls your main logic
}


void CEnemy::Update(float dt) {
	if (!m_pObjectManager || !m_pTileManager || !m_pRenderer) return;
	
	OutputDebugStringA("CEnemy::Update called\n");

	CPlayer* pPlayer = m_pObjectManager->m_pPlayer;
	if (!pPlayer) return;
	Vector2 toPlayer = pPlayer->m_vPos - m_vPos;
	float dist = toPlayer.Length();
	bool hasLOS = m_pTileManager->Visible(m_vPos, pPlayer->m_vPos, pPlayer->GetRadius());
	float attackRange = m_fRadius + pPlayer->GetRadius();
	if (!hasLOS) {
		m_vVelocity = Vector2(0, 0);
		return;
	} // ========================== 
	  // CHASE LOGIC 
	  // // ========================== 
	if (dist > attackRange) {
		Vector2 dir = toPlayer;
		dir.Normalize();

		Vector2 oldPos = m_vPos;

		m_vVelocity = dir * m_fSpeed;
		CObject::move(); bool blocked = (m_vPos - oldPos).Length() < 1.0f;

		m_vPos = oldPos; if (blocked) {
			float base = atan2f(dir.y, dir.x);
			float step = XM_PI / 4.0f;
			float attempts[4] = {
				base + step, base - step,
				base + 2 * step, base - 2 * step
			};

			for (int i = 0; i < 4; i++)
			{
				Vector2 tryDir = AngleToVector(attempts[i]);

				m_vPos = oldPos;
				m_vVelocity = tryDir * m_fSpeed;

				CObject::move();
				if ((m_vPos - oldPos).Length() >= 1.0f) {
					dir = tryDir; break;
				}
			} m_vPos = oldPos;
		} m_vVelocity = dir * m_fSpeed;
		// ========================== 
		// WALK ANIMATION 
		// ========================== 
		bool moving = m_vVelocity.Length() > 0.1f;

		if (moving)
		{
			float ax = fabsf(m_vVelocity.x);
			float ay = fabsf(m_vVelocity.y);

			if (ax > ay)
				m_nSpriteIndex = (m_vVelocity.x > 0) ?
				(UINT)eSprite::SkeletonWalkRightSpriteSheet :
				(UINT)eSprite::SkeletonWalkLeftSpriteSheet;
			else m_nSpriteIndex = (m_vVelocity.y > 0) ?
				(UINT)eSprite::SkeletonWalkUpSpriteSheet :
				(UINT)eSprite::SkeletonWalkDownSpriteSheet;
			if (m_pFrameEvent->Triggered()) {
				UINT frames = m_pRenderer->GetNumFrames(m_nSpriteIndex);

				m_nCurrentFrame = (m_nCurrentFrame + 1) % frames;
			}
		}
		else {
			m_nSpriteIndex = (UINT)eSprite::SkeletonStandDown;
			m_nCurrentFrame = 0;
		}
	}
	else {
		// ========================== 
		// ATTACK LOGIC 
		 //========================== 
		m_vVelocity = Vector2(0, 0);
		m_nSpriteIndex = (UINT)eSprite::SkeletonStandDown;
		if (m_pAttackCooldown->Triggered()) {
			// Attack here later 

		}
	}
}