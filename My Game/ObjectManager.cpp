/// \file ObjectManager.cpp
/// \brief Code for the the object manager class CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"

#include "Player.h"
#include "Zombie.h"
#include "Turret.h"
#include "Bullet.h"
#include "ParticleEngine.h"
#include "Helpers.h"
#include "GameDefines.h"
#include "TileManager.h"
#include "Furniture.h"
#include "Enemy.h"
#include "HealthBar.h"
#include "BulletEnemy.h"
#include "StationaryTurret.h"



/// Create an object and put a pointer to it at the back of the object list
/// `m_stdObjectList`, which it inherits from `LBaseObjectManager`.
/// \param t Sprite type.
/// \param pos Initial position.
/// \return Pointer to the object created.

CObject* CObjectManager::create(eSprite t, const Vector2& pos) {
    CObject* pObj = nullptr;

    if (
        t == eSprite::PlayerStandDown || t == eSprite::PlayerStandUp ||
        t == eSprite::PlayerStandLeft || t == eSprite::PlayerStandRight ||
        t == eSprite::PlayerWalkDown || t == eSprite::PlayerWalkUp ||
        t == eSprite::PlayerWalkLeft || t == eSprite::PlayerWalkRight
        ) {
        
        pObj = new CPlayer(t, pos);
        m_pPlayer = (CPlayer*)pObj;  
    }
    
    else { 
        switch (t) { 
        case eSprite::Turret:  pObj = new CTurret(pos); break;
        case eSprite::stationaryturret:pObj = new CStationaryTurret(pos); break;
        case eSprite::bulletenemy: pObj = new CBulletEnemy(eSprite::bulletenemy, pos); break;
        case eSprite::ZombieStandDown: pObj = new CZombie(pos); break;
        case eSprite::Bullet:  pObj = new CBullet(eSprite::Bullet, pos); break;
        //case eSprite::Bullet2: pObj = new CBullet(eSprite::Bullet2, pos); break;
        case eSprite::Fireball: pObj = new CBullet(eSprite::Fireball, pos); break;
        case eSprite::sword: pObj = new CBullet(eSprite::sword, pos); break;
        case eSprite::greatsword: pObj = new CBullet(eSprite::greatsword, pos); break;
        case eSprite::dagger: pObj = new CBullet(eSprite::dagger, pos); break;
        case eSprite::shield: pObj = new CObject(eSprite::shield, pos); break;
        default: pObj = new CObject(t, pos); break;

        } 
    }

    m_stdObjectList.push_back(pObj); 
    return pObj;
} 

CObject* CObjectManager::createFurniture(eSprite t, const Vector2& pos, char type) {
    CObject* pObj = nullptr;
    if (type == 'H')
    {
		pObj = new CHealthBar(pos);
		pObj->SetSprite(eSprite::HealthBar);
		m_stdObjectList.push_back(pObj); //push pointer onto object list
		return pObj; //return pointer to created object
    }


    pObj = new CFurniture(pos);
    pObj->SetSprite(t);
	pObj->SetFrame(t, type);

    m_stdObjectList.push_back(pObj); //push pointer onto object list
    return pObj; //return pointer to created object
} //create furniture



/// Draw the tiled background and the objects in the object list.

void CObjectManager::draw(){
    for (CObject* pObj : m_stdObjectList) {
        std::cout << "[Draw] Obj @" << pObj
            << " | Sprite: " << pObj->m_nSpriteIndex
            << " | Dead: " << pObj->m_bDead << std::endl;

        if (!pObj->m_bDead)
            pObj->draw(); // normal draw call
    }


  m_pTileManager->Draw(eSprite::Tile); //draw tiled background

  if(m_bDrawAABBs)
    m_pTileManager->DrawBoundingBoxes(eSprite::Line); //draw AABBs
  for (CObject* pObj : m_stdObjectList)
  {
      if (pObj->isHealthBar && m_pPlayer) {
          const UINT spriteIndex = pObj->m_nSpriteIndex;
          const size_t numFrames = m_pRenderer->GetNumFrames(spriteIndex);
          if (numFrames == 0) continue; // nothing loaded

          // Replace these with getters on CPlayer (add if necessary)
          const float health = (float)m_pPlayer->GetHealth();        // implement GetHealth()
          const float maxHealth = (float)m_pPlayer->GetMaxHealth();  // implement GetMaxHealth()
          const float ratio = health / maxHealth;

          int frame = (int)floor(ratio * 100);
          frame -= frame % 5;
          frame /= 5;

		  if (frame < 0) frame = 0;

          pObj->m_nCurrentFrame = frame;

          // position above player in sprite units (use tile/sprite extents, not 1.0f)
          pObj->m_vPos = m_pPlayer->m_vPos - Vector2(0.0f, -m_pRenderer->GetHeight(m_pPlayer->m_nSpriteIndex) * 0.5f - 10.0f);
      }
  }
  LBaseObjectManager::draw();
} //draw

/// Perform collision detection and response for each object with the world
/// edges and for all objects with another object, making sure that each pair
/// of objects is processed only once.

void CObjectManager::BroadPhase()
{
    LBaseObjectManager::BroadPhase();

    float dt = m_pTimer->GetFrameTime();

    for (CObject* pObj : m_stdObjectList)
    {
        if (!pObj->m_bDead)
        {
            pObj->Update(dt);

            for (int i = 0; i < 2; i++)
            {
                Vector2 norm; float d = 0;
                BoundingSphere s(Vector3(pObj->m_vPos), pObj->m_fRadius);

                if (m_pTileManager->CollideWithWall(s, norm, d))
                    pObj->CollisionResponse(norm, d);
            }
        }
    }
}



/// Perform collision detection and response for a pair of objects. Makes
/// use of the helper function Identify() because this function may be called
/// with the objects in an arbitrary order.
/// \param p0 Pointer to the first object.
/// \param p1 Pointer to the second object.

void CObjectManager::NarrowPhase(CObject* p0, CObject* p1){
  Vector2 vSep = p0->m_vPos - p1->m_vPos; //vector from *p1 to *p0
  const float d = p0->m_fRadius + p1->m_fRadius - vSep.Length(); //overlap

  if (p0->isFurniture || p1->isFurniture)
  {
	  return; // Furniture does not collide
  }

  if(d > 0.0f){ //bounding circles overlap
    vSep.Normalize(); //vSep is now the collision normal

    p0->CollisionResponse( vSep, d, p1); //this changes separation of objects
    p1->CollisionResponse(-vSep, d, p0); //same separation and opposite normal
  } //if
} //NarrowPhase

/// Create a bullet object and a flash particle effect. It is assumed that the
/// object is round and that the bullet appears at the edge of the object in
/// the direction that it is facing and continues moving in that direction.
/// \param pObj Pointer to an object.
/// \param bullet Sprite type of bullet.

void CObjectManager::Add(CObject* p)
{
    if (!p) return;
    m_stdObjectList.push_back(p);

    // Track enemies if needed
    if (CEnemy* pEnemy = dynamic_cast<CEnemy*>(p)) {
        m_enemies.push_back(pEnemy);
    }
}

void CObjectManager::FireGun(CPlayer* pPlayer, eSprite t, const Vector2& vDir) {
    float fSpeed = 500.0f;
    float fLifeSpan = 0.0f;

    const float w0 = 0.5f * m_pRenderer->GetWidth(pPlayer->m_nSpriteIndex);
    const float w1 = m_pRenderer->GetWidth(t);
    float fLaunchDistance = w0 + w1;

    if (t == eSprite::Fireball) {
        fSpeed = FIREBALL_SPEED;
        fLifeSpan = 5.0f;
    }
    else if (t == eSprite::sword) {
        fSpeed = SWORD_SPEED;
        fLifeSpan = SWORD_LIFESPAN;

        fLaunchDistance = SWORD_OFFSET;

        m_pAudio->stop(eSound::Gun);
        m_pAudio->play(eSound::Clang);
    }
    else if (t == eSprite::greatsword) {
        fSpeed = GREATSWORD_SPEED;
        fLifeSpan = GREATSWORD_LIFESPAN;
        fLaunchDistance = GREATSWORD_OFFSET;

        m_pAudio->stop(eSound::Gun);
        m_pAudio->play(eSound::Boom);
    }

    else if (t == eSprite::dagger) {
        fSpeed = DAGGER_SPEED;
        fLifeSpan = DAGGER_LIFESPAN;
        fLaunchDistance = DAGGER_OFFSET;

        m_pAudio->stop(eSound::Gun);
        m_pAudio->play(eSound::Clang);
    }
    else {

        m_pAudio->play(eSound::Gun);
    }


    const Vector2 pos = pPlayer->m_vPos + fLaunchDistance * vDir;


    CObject* pBullet = create(t, pos);

    pBullet->m_fMaxLifeSpan = fLifeSpan;
    pBullet->m_fTimeAlive = 0.0f;

    const Vector2 norm = VectorNormalCC(vDir);
    const float m = 2.0f * m_pRandom->randf() - 1.0f;
    const Vector2 deflection = 0.01f * m * norm;

    pBullet->m_vVelocity = pPlayer->m_vVelocity + fSpeed * (vDir + deflection);
    pBullet->m_fRoll = pPlayer->m_fRoll;


    LParticleDesc2D d;

    if (t != eSprite::Fireball && t != eSprite::sword) {
        d.m_nSpriteIndex = (UINT)eSprite::Spark;
        d.m_vPos = pos;
        d.m_vVel = pPlayer->m_fSpeed * vDir;
        d.m_fLifeSpan = 0.25f;
        d.m_fScaleInFrac = 0.4f;
        d.m_fFadeOutFrac = 0.5f;
        d.m_fMaxScale = 0.5f;
        d.m_f4Tint = XMFLOAT4(Colors::Yellow);
        m_pParticleEngine->create(d);
    }

    if (t == eSprite::Fireball) {
        d.m_nSpriteIndex = (UINT)eSprite::Smoke;
        d.m_vPos = pos;
        d.m_vVel = pPlayer->m_fSpeed * vDir * 0.5f;
        d.m_fLifeSpan = 0.5f;
        d.m_fScaleInFrac = 0.4f;
        d.m_fFadeOutFrac = 0.5f;
        d.m_fMaxScale = 1.0f;
        d.m_f4Tint = XMFLOAT4(Colors::Red);
        m_pParticleEngine->create(d);
    }
}




// For StationaryTurret
void CObjectManager::FireGun(CObject* pObj, eSprite bullet) {
    if (!pObj) return;

    const Vector2 vDir = pObj->GetViewVector();



    m_pAudio->play(eSound::Gun);

    const float w0 = 0.5f * m_pRenderer->GetWidth(pObj->m_nSpriteIndex);
    const float w1 = m_pRenderer->GetWidth(bullet);
    const Vector2 pos = pObj->m_vPos + (w0 + w1) * vDir;

    CObject* pBullet = create(bullet, pos);

    const Vector2 norm = VectorNormalCC(vDir);
    const float m = 2.0f * m_pRandom->randf() - 1.0f;
    const Vector2 deflection = 0.01f * m * norm;

    float fEnemySpeed = 0.0f;
    float fBulletSpeed = 500.0f;

    pBullet->m_vVelocity = fEnemySpeed * vDir + fBulletSpeed * (vDir + deflection);
    pBullet->m_fRoll = pObj->m_fRoll;

    LParticleDesc2D d;
    d.m_nSpriteIndex = (UINT)eSprite::Spark;
    d.m_vPos = pos;
    d.m_vVel = fEnemySpeed * vDir;
    d.m_fLifeSpan = 0.25f;
    d.m_fScaleInFrac = 0.4f;
    d.m_fFadeOutFrac = 0.5f;
    d.m_fMaxScale = 0.5f;
    d.m_f4Tint = XMFLOAT4(Colors::Yellow);
    m_pParticleEngine->create(d);
}

/// Reader function for the number of Zombies. 
/// \return Number of Zombies in the object list.


const size_t CObjectManager::GetNumTurrets() const{
  size_t n = 0; 
  
  for(CObject* pObj: m_stdObjectList)
    if(pObj->m_nSpriteIndex == (UINT)eSprite::Turret)
      n++;

  return n;
} //GetNumZombies