#include "StationaryTurret.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"


CStationaryTurret::CStationaryTurret(const Vector2& p) : CObject(eSprite::Turret, p) {
	m_bStatic = true; 
	m_fRoll = 0.0f; 
	m_fRotSpeed = 0.0f; 
	m_fSpeed = 0.0f;
}


void CStationaryTurret::move() {
	

	if (m_pGunFireEvent->Triggered())
		m_pObjectManager->FireGun(this, eSprite::bulletenemy); 
}



void CStationaryTurret::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
	if (m_bDead)return; 

	if (pObj && pObj->isBullet()) { 
		if (--m_nHealth == 0) { 
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
} 


void CStationaryTurret::DeathFX() {
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

