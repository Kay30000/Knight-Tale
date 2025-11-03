/// \file Player.cpp
/// \brief Code for the player object class CPlayer.

#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"

/// Create and initialize an player object given its initial position.
/// \param p Initial position of player.

CPlayer::CPlayer(const Vector2& p): CObject(eSprite::Player, p){ 
  m_bIsTarget = true;
  m_bStatic = false;
} //constructor

/// Move and rotate in response to device input. The amount of motion and
/// rotation speed is proportional to the frame time.

void CPlayer::move(){
  const float t = m_pTimer->GetFrameTime(); //time
  const Vector2 view = GetViewVector(); //view vector
  m_vPos += m_fSpeed*t*view; //move forwards
  m_fRoll += m_fRotSpeed*t; //rotate
  NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy

  //strafe
    
  const Vector2 norm = VectorNormalCC(view); //normal to view vector
  const float delta = 40.0f*t; //change in position for strafing

  if(m_bStrafeRight)m_vPos += delta*norm; //strafe right
  else if(m_bStrafeLeft)m_vPos -= delta*norm; //strafe left
  else if(m_bStrafeBack)m_vPos -= delta*view; //strafe back

  m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = false; //reset strafe flags
} //move

/// Response to collision. If the object being collided with is a bullet, then
/// play a sound, otherwise call `CObject::CollisionResponse` for the default
/// collision response.
/// \param norm Collision normal.
/// \param d Overlap distance.
/// \param pObj Pointer to object being collided with (defaults to `nullptr`,
/// which means collision with a wall).

void CPlayer::CollisionResponse(const Vector2& norm, float d, CObject* pObj){
  if(m_bDead)return; //already dead, bail out 

  if(pObj && pObj->isBullet()){ //collision with bullet
    if(m_bGodMode) //god mode, does no damage
      m_pAudio->play(eSound::Grunt); //impact sound

    else if(--m_nHealth == 0){ //health decrements to zero means death 
      m_pAudio->play(eSound::Boom); //explosion
      m_bDead = true; //flag for deletion from object list
      DeathFX(); //particle effects
      m_pPlayer = nullptr; //clear common player pointer
    } //if

    else{ //not a death blow
      m_pAudio->play(eSound::Grunt); //impact sound
      const float f = 0.5f + 0.5f*(float)m_nHealth/m_nMaxHealth; //health fraction
      m_f4Tint = XMFLOAT4(1.0f, f, f, 0); //redden the health indicator
    } //else
  } //if

  CObject::CollisionResponse(norm, d, pObj); //default collision response
} //CollisionResponse

/// Perform a particle effect to mark the death of the player.

void CPlayer::DeathFX(){
  LParticleDesc2D d; //particle descriptor
  d.m_vPos = m_vPos; //center particle at player center

  d.m_nSpriteIndex = (UINT)eSprite::Smoke;
  d.m_fLifeSpan = 2.0f;
  d.m_fMaxScale = 4.0f;
  d.m_fScaleInFrac = 0.5f;
  d.m_fFadeOutFrac = 0.8f;
  d.m_fScaleOutFrac = 0;
  m_pParticleEngine->create(d);

  d.m_nSpriteIndex = (UINT)eSprite::Spark;
  d.m_fLifeSpan = 0.5f;
  d.m_fMaxScale = 1.5f;
  d.m_fScaleInFrac = 0.4f;
  d.m_fScaleOutFrac = 0.3f;
  d.m_fFadeOutFrac = 0.5f;
  d.m_f4Tint = XMFLOAT4(Colors::OrangeRed);
  m_pParticleEngine->create(d);
} //DeathFX

/// Set the strafe left flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeLeft(){
  m_bStrafeLeft = true;
} //StrafeLeft

/// Set the strafe right flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeRight(){
  m_bStrafeRight = true;
} //StrafeRight

/// Set the strafe back flag. This function will be called in response to
/// device inputs.

void CPlayer::StrafeBack(){
  m_bStrafeBack = true;
} //StrafeBack

/// Set the object's speed, assuming that the object moves in the direction of
/// its view vector. This function will be called in response to device inputs.
/// \param speed Speed.

void CPlayer::SetSpeed(const float speed){
  m_fSpeed = speed;
} //SetSpeed

/// Set the object's rotational speed in revolutions per second. This function
/// will be called in response to device inputs.
/// \param speed Rotational speed in RPS.

void CPlayer::SetRotSpeed(const float speed){
  m_fRotSpeed = speed;
} //SetRotSpeed

/// Reader function for position.
/// \return Position.

const Vector2& CPlayer::GetPos() const{
  return m_vPos;
} //GetPos
