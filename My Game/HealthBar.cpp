#include "HealthBar.h"
#include "ComponentIncludes.h"
#include "Helpers.h"

CHealthBar::CHealthBar(const Vector2& p, int maxHealth) : CObject(eSprite::HealthBar, Vector2(p)) {
	this->maxHealth = maxHealth;
	playerHealth = maxHealth;
	isFurniture = true;
	m_bStatic = false;
	m_nCurrentFrame = currentFrame;
}

int CHealthBar::getFrame()
{
	float frame = playerHealth / (float)maxHealth * 100;
	frame = floor(frame);
	int frame1 = (int)(frame) - ((int)(frame) % 5);
	return frame1;
}

void CHealthBar::move(int c) {
	const float t = m_pTimer->GetFrameTime();
	const float baseSpeed = 200.0f;
	const float delta = baseSpeed * t;

	m_fRotSpeed = 0.0f;
	NormalizeAngle(m_fRoll);

	
	// W Forward/Up movement 
	if (c == 0) {
		m_vPos.y += delta;
	}

	// S Backward/Down movement 
	else if (c == 1) {
		m_vPos.y -= delta;
	}

	// D Right movement
	if (c == 2) {
		m_vPos.x += delta;
	}

	// A Left movement
	else if (c == 3) {
		m_vPos.x -= delta;
	}

	

}