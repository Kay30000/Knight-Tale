
#ifndef __L4RC_GAME_STATIONARYTURRET_H__
#define __L4RC_GAME_STATIONARYTURRET_H__

#include "Object.h"
#include "Enemy.h"

class CStationaryTurret : public CObject {
protected:
	const UINT m_nMaxHealth = 8;
	UINT m_nHealth = m_nMaxHealth; 

	virtual void CollisionResponse(const Vector2& norm, float d, CObject* pObj = nullptr); 
	virtual void DeathFX(); 

public:
	CStationaryTurret(const Vector2& p);
	virtual void move(); 
}; 

#endif 