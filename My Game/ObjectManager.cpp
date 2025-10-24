/// \file ObjectManager.cpp
/// \brief Code for the the object manager class CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"
#include "Player.h"

/// Create an object and put a pointer to it at the back of the object list
/// `m_stdObjectList`, which it inherits from `LBaseObjectManager`.
/// \param t Sprite type.
/// \param pos Initial position.
/// \return Pointer to the object created.

CObject* CObjectManager::create(eSprite t, const Vector2& pos){
	CObject* pObj = nullptr;

    switch (t) {
    case eSprite::PlayerWalkLeft:
        pObj = new CPlayer(eSprite::PlayerWalkLeft, pos);
        break;

    case eSprite::PlayerWalkRight:
        pObj = new CPlayer(eSprite::PlayerWalkRight, pos);
        break;

    case eSprite::PlayerStandLeft:
        pObj = new CPlayer(eSprite::PlayerStandLeft, pos);
        break;

    case eSprite::PlayerStandRight:
        pObj = new CPlayer(eSprite::PlayerStandRight, pos);
        break;

    default: pObj = new CObject(t, pos);
    } //switch


  m_stdObjectList.push_back(pObj); //push pointer onto object list
  return pObj; //return pointer to created object
} //create
