/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include "BaseObjectManager.h"
#include "Object.h"
#include "Common.h"

class CEnemy;

class CObjectManager: 
  public LBaseObjectManager<CObject>,
  public CCommon
{
  private:
    void BroadPhase(); ///< Broad phase collision detection and response.
    void NarrowPhase(CObject*, CObject*); ///< Narrow phase collision detection and response.
    
  public:
    CObject* create(eSprite, const Vector2&); ///< Create new object.
	CObject* createFurniture(eSprite, const Vector2&, char); ///< Create new furniture object.
    CPlayer* m_pPlayer = nullptr;
    std::vector<CEnemy*> m_enemies;
    void Add(CObject* p); ///< Add an object to the manager.

    virtual void draw(); ///< Draw all objects.


    void FireGun(CPlayer*, eSprite, const Vector2& vDir);
    const size_t GetNumTurrets() const; 
}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__
