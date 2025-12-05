#include "Object.h"

class CPickUp : public CObject {
protected:
	bool m_bCollected = false;
	virtual void CollisionResponse(const Vector2&, float, CObject* = nullptr); ///< Collision response.
	virtual void DeathFX(); ///< Death special effects.
public:
	
	eSprite pickupSprite;
	CPickUp(const Vector2& p);
}; //CPickUp