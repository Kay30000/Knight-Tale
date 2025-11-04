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

  Background, Bullet, Bullet2, Turret, Smoke, Spark, Tile, Line, Furniture, Fireball, PlayerWalkLeftSpriteSheet, PlayerWalkRightSpriteSheet,
PlayerStandLeftSpriteSheet, PlayerStandRightSpriteSheet, HealthBar, HealthBarSheet,
PlayerWalkLeft, PlayerWalkRight, PlayerStandLeft, PlayerStandRight, PlayerWalkUpSpriteSheet, PlayerWalkDownSpriteSheet,
PlayerStandUpSpriteSheet, PlayerStandDownSpriteSheet,
PlayerWalkUp, PlayerWalkDown, PlayerStandUp, PlayerStandDown, PlayerAttackLeft, 
PlayerAttackRight, PlayerAttackUp, PlayerAttackDown,
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



//FireBall
const int FIREBALL_DAMAGE = 5;
const float FIREBALL_SPEED = 500.0f;

#endif //__L4RC_GAME_GAMEDEFINES_H__
