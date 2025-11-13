#include "HealthBar.h"
#include "ComponentIncludes.h"
#include "Helpers.h"

CHealthBar::CHealthBar(const Vector2& p) : CObject(eSprite::HealthBar, Vector2(p)) {
	isHealthBar = true;
	isFurniture = true;
	m_bStatic = false;
	m_bDead = false;
	m_nCurrentFrame = currentFrame;
}



