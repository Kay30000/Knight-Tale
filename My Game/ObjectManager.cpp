/// \file ObjectManager.cpp
/// \brief Code for the the object manager class CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"

#include "Player.h"
#include "Turret.h"
#include "Bullet.h"
#include "ParticleEngine.h"
#include "Helpers.h"
#include "GameDefines.h"
#include "TileManager.h"

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
    }
    else { 
        switch (t) { 
        case eSprite::Turret:  pObj = new CTurret(pos); break;
        case eSprite::Bullet:  pObj = new CBullet(eSprite::Bullet, pos); break;
        case eSprite::Bullet2: pObj = new CBullet(eSprite::Bullet2, pos); break;
        case eSprite::Fireball: pObj = new CBullet(eSprite::Fireball, pos); break;
        default: pObj = new CObject(t, pos);
        } 
    }

    m_stdObjectList.push_back(pObj); 
    return pObj;
} 

/// Draw the tiled background and the objects in the object list.

void CObjectManager::draw(){
  m_pTileManager->Draw(eSprite::Tile); //draw tiled background

  if(m_bDrawAABBs)
    m_pTileManager->DrawBoundingBoxes(eSprite::Line); //draw AABBs

  LBaseObjectManager::draw();
} //draw

/// Perform collision detection and response for each object with the world
/// edges and for all objects with another object, making sure that each pair
/// of objects is processed only once.

void CObjectManager::BroadPhase(){
  LBaseObjectManager::BroadPhase(); //collide with other objects

  //collide with walls

  for(CObject* pObj: m_stdObjectList) //for each object
    if(!pObj->m_bDead){ //for each non-dead object, that is
      for(int i=0; i<2; i++){ //can collide with 2 edges simultaneously
        Vector2 norm; //collision normal
        float d = 0; //overlap distance
        BoundingSphere s(Vector3(pObj->m_vPos), pObj->m_fRadius);
        
        if(m_pTileManager->CollideWithWall(s, norm, d)) //collide with wall
          pObj->CollisionResponse(norm, d); //respond 
      } //for
  } //for
} //BroadPhase

/// Perform collision detection and response for a pair of objects. Makes
/// use of the helper function Identify() because this function may be called
/// with the objects in an arbitrary order.
/// \param p0 Pointer to the first object.
/// \param p1 Pointer to the second object.

void CObjectManager::NarrowPhase(CObject* p0, CObject* p1){
  Vector2 vSep = p0->m_vPos - p1->m_vPos; //vector from *p1 to *p0
  const float d = p0->m_fRadius + p1->m_fRadius - vSep.Length(); //overlap

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

void CObjectManager::FireGun(CPlayer* pPlayer, eSprite t, const Vector2& vDir) {
    m_pAudio->play(eSound::Gun);

    float fSpeed = 500.0f; 

    if (t == eSprite::Fireball) {
        fSpeed = FIREBALL_SPEED;
    }
    const float w0 = 0.5f * m_pRenderer->GetWidth(pPlayer->m_nSpriteIndex);
    const float w1 = m_pRenderer->GetWidth(t);
    const Vector2 pos = pPlayer->m_vPos + (w0 + w1) * vDir; 

   
    CObject* pBullet = create(t, pos); 

    const Vector2 norm = VectorNormalCC(vDir); 
    const float m = 2.0f * m_pRandom->randf() - 1.0f;
    const Vector2 deflection = 0.01f * m * norm; 

    pBullet->m_vVelocity = pPlayer->m_vVelocity + fSpeed * (vDir + deflection);
    pBullet->m_fRoll = pPlayer->m_fRoll;


    LParticleDesc2D d;

    d.m_nSpriteIndex = (UINT)eSprite::Spark;
    d.m_vPos = pos;
    d.m_vVel = pPlayer->m_fSpeed * vDir;
    d.m_fLifeSpan = 0.25f;
    d.m_fScaleInFrac = 0.4f;
    d.m_fFadeOutFrac = 0.5f;
    d.m_fMaxScale = 0.5f;
    d.m_f4Tint = XMFLOAT4(Colors::Yellow);

    m_pParticleEngine->create(d);

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

/// Reader function for the number of turrets. 
/// \return Number of turrets in the object list.

const size_t CObjectManager::GetNumTurrets() const{
  size_t n = 0; 
  
  for(CObject* pObj: m_stdObjectList) 
    if(pObj->m_nSpriteIndex == (UINT)eSprite::Turret)
      n++;

  return n;
} //GetNumTurrets