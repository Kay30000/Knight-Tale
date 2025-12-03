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

  Background, Bullet, bulletenemy, Turret, Smoke, Spark, Tile, Line, Furniture, Fireball, sword, greatsword, dagger, shield, HealthBar, stationaryturret,
  
//Player Walk 
PlayerWalkLeft, PlayerWalkLeftSpriteSheet, PlayerWalkRight, 
PlayerWalkRightSpriteSheet, PlayerWalkUp, PlayerWalkUpSpriteSheet, 
PlayerWalkDown, PlayerWalkDownSpriteSheet,  
// Player Stand
PlayerStandLeft, PlayerStandRight, 
PlayerStandUp, PlayerStandDown, 
//Player Attack
PlayerAttackLeft, PlayerAttackLeftSpriteSheet,

PlayerAttackRight, PlayerAttackRightSpriteSheet, PlayerAttackUp, 
PlayerAttackUpSpriteSheet, PlayerAttackDown, PlayerAttackDownSpriteSheet,
//Zombie Stand
ZombieStandDown, ZombieStandLeft, ZombieStandRight, ZombieStandUp,
//Zombie Walk
ZombieWalkLeftSpriteSheet, ZombieWalkLeft,
ZombieWalkRightSpriteSheet, ZombieWalkRight,
ZombieWalkUpSpriteSheet, ZombieWalkUp,
ZombieWalkDownSpriteSheet, ZombieWalkDown,

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
  Playing, Waiting, Paused
}; //eGameState



// FireBall
const int FIREBALL_DAMAGE = 5;
const float FIREBALL_SPEED = 500.0f;

// Sword
const int SWORD_DAMAGE = 2;
const float SWORD_SPEED = 1000.0f;
const float SWORD_LIFESPAN = 0.1f;
const float SWORD_OFFSET = 67.0f;;

// Great Sword
const int GREATSWORD_DAMAGE = 5;
const float GREATSWORD_SPEED = 500.0f;
const float GREATSWORD_LIFESPAN = 0.15f;
const float GREATSWORD_OFFSET = 67.0f;

// Dagger
const int DAGGER_DAMAGE = 1;
const float DAGGER_SPEED = 1500.0f;
const float DAGGER_LIFESPAN = 0.05f;
const float DAGGER_OFFSET = 67.0f;

// Cooldowns
const float COOLDOWN_BULLET = 0.5f;
const float COOLDOWN_FIREBALL = 1.2f;
const float COOLDOWN_SWORD = 0.5f;
const float COOLDOWN_GREATSWORD = 1.5f;
const float COOLDOWN_DAGGER = 0.2f;

// Shield 
const float SHIELD_OFFSET = 55.0f;
const float SHIELD_DURATION = 0.0f;
const float SHIELD_RADIUS_MODIFIER = 1.0f;

const float PLAYER_NORMAL_SPEED = 300.0f;
const float PLAYER_SHIELD_SPEED = 100.0f;


#endif //__L4RC_GAME_GAMEDEFINES_H__
