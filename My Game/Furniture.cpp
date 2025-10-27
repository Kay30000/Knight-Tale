#include "Furniture.h"
#include "ComponentIncludes.h"
#include "ObjectManager.h"
#include "TileManager.h"
#include "Player.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"





CFurniture::CFurniture(const Vector2& p):CObject(eSprite::Cowskull,p)
{
	m_bStatic = true;
}