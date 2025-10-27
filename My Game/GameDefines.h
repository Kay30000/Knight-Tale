/// \file GameDefines.h
/// \brief Game specific defines.

#ifndef __L4RC_GAME_GAMEDEFINES_H__
#define __L4RC_GAME_GAMEDEFINES_H__

#include "Defines.h"

/// \brief Sprite enumerated type.
///
/// An enumerated type for the sprites, which will be cast to an unsigned
/// integer and used for the index of the corresponding texture in graphics
/// memory. `Size` must be last.

enum class eSprite: UINT{
  Background, Bullet, Bullet2, Player, PlayerLeft, Turret, Smoke, Spark, Tile, Line, fireball,
  Size  //MUST BE LAST
}; //eSprite

/// \brief Sound enumerated type.
///
/// An enumerated type for the sounds, which will be cast to an unsigned
/// integer and used for the index of the corresponding sample. `Size` must 
/// be last.

enum class eSound: UINT{
  Start, Boom, Clang, Grunt, Gun, Ricochet,
  Size  //MUST BE LAST
}; //eSound

/// \brief Game state enumerated type.
///
/// An enumerated type for the game state, which can be either playing or
/// waiting a short interval for the level to end after winning or losing to
/// let sounds and particle effects play to the end without being cut short.

enum class eGameState{
  Playing, Waiting
}; //eGameState


// Sword
const float SWORD_SWING_DURATION = 0.2f;
const float SWORD_RANGE = 100.0f;
const int SWORD_DAMAGE = 3;

//Dagger
const float DAGGER_SWING_DURATION = 0.1f;
const float DAGGER_RANGE = 50.0f;
const int DAGGER_DAMAGE = 1;

//Great Sword
const float GREATSWORD_SWING_DURATION = 0.3f;
const float GREATSWORD_RANGE = 150.0f;
const int GREATSWORD_DAMAGE = 5;

//FireBall
const int FIREBALL_DAMAGE = 5;
const float FIREBALL_SPEED = 500.0f;



#endif //__L4RC_GAME_GAMEDEFINES_H__
