#include "BulletEnemy.h"
#include "ComponentIncludes.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "Helpers.h"

CBulletEnemy::CBulletEnemy(eSprite t, const Vector2& p) : CObject(t, p) {
	m_bIsBullet = true;
	m_bStatic = false;
	m_bIsTarget = false;
} 

void CBulletEnemy::CollisionResponse(const Vector2& norm, float d, CObject* pObj) {
	if (pObj == nullptr) 
		m_pAudio->play(eSound::Ricochet);


	if (!m_bDead) {
		m_bDead = true;
		DeathFX();
	} 
} 


void CBulletEnemy::DeathFX() {
	LParticleDesc2D d; 

	d.m_nSpriteIndex = (UINT)eSprite::Smoke;
	d.m_vPos = m_vPos;
	d.m_fLifeSpan = 0.5f;
	d.m_fMaxScale = 0.5f;
	d.m_fScaleInFrac = 0.2f;
	d.m_fFadeOutFrac = 0.8f;
	d.m_fScaleOutFrac = d.m_fFadeOutFrac;

	m_pParticleEngine->create(d); 
} 