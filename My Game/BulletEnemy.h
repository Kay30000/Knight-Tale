#ifndef __L4RC_GAME_BULLETENEMY_H__
#define __L4RC_GAME_BULLETENEMY_H__

#include "Object.h"


class CBulletEnemy : public CObject {
protected:
	virtual void CollisionResponse(const Vector2&, float d, CObject* = nullptr); 
	virtual void DeathFX(); 

public:
	CBulletEnemy(eSprite t, const Vector2& p); 
}; 

#endif 