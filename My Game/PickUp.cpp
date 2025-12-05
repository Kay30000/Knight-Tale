#include "PickUp.h"
#include "ComponentIncludes.h"

CPickUp::CPickUp(const Vector2& p)
  : CObject(eSprite::Pickup, p){
  m_bStatic = true; //pickups are static
  isPickup = true;
  variant = 0; //currently only one type of pickup, plan to make more later and set each pickup to a random type
  m_nCurrentFrame = variant;
} //constructor

CPickUp::CPickUp(const Vector2& p, int var)
  : CObject(eSprite::Pickup, p){
  m_bStatic = true; //pickups are static
  isPickup = true;
  variant = var; //set pickup to specified variant
  m_nCurrentFrame = variant;
} //constructor with variant parameter

void CPickUp::CollisionResponse(const Vector2& norm, float d, CObject* pObj)
{
	if (m_bDead)return;

	if (pObj && pObj->isPlayer && !m_bDead) {
		m_bCollected = true; //pickups disappear on contact with the player, effect on player handled in player class
		DeathFX();
	}
	else
	{
		return; //no collision response for anything other than the player
	}

	if (m_bCollected)
	{
		m_bDead = true; //mark object for deletion
	}
}

void CPickUp::DeathFX()
{
	// Might add sound fx later
}