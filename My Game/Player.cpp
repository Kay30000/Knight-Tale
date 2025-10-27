#include "Player.h"
#include "ComponentIncludes.h"
#include "Helpers.h"
#include "Particle.h"
#include "ParticleEngine.h"
#include "ObjectManager.h"

/// Create and initialize an player object given its initial position.
/// \param p Initial position of player.

CPlayer::CPlayer(eSprite t, const Vector2& p): CObject(eSprite::Player, p){ 
  m_bIsTarget = true;
  m_bStatic = false;

  //Sword
  m_fSwingTimer = 0.0f;
  m_bSwordAttacked = false;

  //Dagger
  m_fDaggerTimer = 0.0f;
  m_bDaggerAttacked = false;

  //GreatSword
  m_fGreatSwordTimer = 0.0f;
  m_bGreatSwordAttacked = false;
  
} 

void CPlayer::move(){

    if (m_fSpeed < 0.0f) {
        SetSprite(eSprite::PlayerLeft);
        m_bFacingLeft = true;
    }
    else if (m_fSpeed > 0.0f) {
        SetSprite(eSprite::Player);
        m_bFacingLeft = false;

    }

  const float t = m_pTimer->GetFrameTime(); //time
  const Vector2 view = GetViewVector(); //view vector
  m_vPos += m_fSpeed*t*view; //move forwards
  m_fRoll += m_fRotSpeed*t; //rotate
  NormalizeAngle(m_fRoll); //normalize to [-pi, pi] for accuracy    
  const Vector2 norm = VectorNormalCC(view); //normal to view vector
  const float delta = 200.0f*t; //change in position for strafing

  if(m_bStrafeRight)m_vPos += delta*norm; //strafe right
  else if(m_bStrafeLeft)m_vPos -= delta*norm; //strafe left
  else if(m_bStrafeBack)m_vPos -= delta*view; //strafe back

  m_bStrafeLeft = m_bStrafeRight = m_bStrafeBack = false; //reset strafe flags

  //Sword
  if (m_fSwingTimer > 0.0f) {
      m_fSwingTimer -= t;

      if (!m_bSwordAttacked && m_fSwingTimer > SWORD_SWING_DURATION - t) {
          const Vector2 view = GetViewVector();
          const Vector2 swordPos = m_vPos + view * SWORD_RANGE;

          if (m_pObjectManager->CheckSwordHit(swordPos, SWORD_DAMAGE)) {
              m_bSwordAttacked = true;
              m_pAudio->play(eSound::Clang);
          }
      }

      else if (m_fSwingTimer <= 0.0f) {
          m_fSwingTimer = 0.0f;
          m_bSwordAttacked = false;
      }
  }

  //Dagger
  if (m_fDaggerTimer > 0.0f) {
      m_fDaggerTimer -= t;

      if (!m_bDaggerAttacked && m_fDaggerTimer > DAGGER_SWING_DURATION - t) {
          const Vector2 view = GetViewVector();
          
          const Vector2 point_near = m_vPos + view * (DAGGER_RANGE * 0.25f);
          const Vector2 point_mid = m_vPos + view * (DAGGER_RANGE * 0.5f);
          const Vector2 point_tip = m_vPos + view * DAGGER_RANGE;

          bool hit_enemy = false;

          if (m_pObjectManager->CheckSwordHit(point_near, DAGGER_DAMAGE)) {
              hit_enemy = true;
          }
          if (m_pObjectManager->CheckSwordHit(point_mid, DAGGER_DAMAGE)) {
              hit_enemy = true;
          }
          if (m_pObjectManager->CheckSwordHit(point_tip, DAGGER_DAMAGE)) {
              hit_enemy = true;
          }
          if (hit_enemy) {
              m_bDaggerAttacked = true;
              m_pAudio->play(eSound::Clang);
          }
      }

      else if (m_fDaggerTimer <= 0.0f) {
          m_fDaggerTimer = 0.0f;
          m_bDaggerAttacked = false;
      }
  }

  //GreatSword
  if (m_fGreatSwordTimer > 0.0f) {
      m_fGreatSwordTimer -= t;

      if (!m_bGreatSwordAttacked && m_fGreatSwordTimer > GREATSWORD_SWING_DURATION - t) {
          const Vector2 view = GetViewVector();

          const Vector2 point_near = m_vPos + view * (GREATSWORD_RANGE * 0.25f);
          const Vector2 point_mid = m_vPos + view * (GREATSWORD_RANGE * 0.5f);
          const Vector2 point_tip = m_vPos + view * GREATSWORD_RANGE;

          bool hit_enemy = false;

          if (m_pObjectManager->CheckSwordHit(point_near, GREATSWORD_DAMAGE)) {
              hit_enemy = true;
          }
          if (m_pObjectManager->CheckSwordHit(point_mid, GREATSWORD_DAMAGE)) {
              hit_enemy = true;
          }
          if (m_pObjectManager->CheckSwordHit(point_tip, GREATSWORD_DAMAGE)) {
              hit_enemy = true;
          }
          if (hit_enemy) {
              m_bGreatSwordAttacked = true;
              m_pAudio->play(eSound::Clang);
          }
      }

      else if (m_fGreatSwordTimer <= 0.0f) {
          m_fGreatSwordTimer = 0.0f;
          m_bGreatSwordAttacked = false;
      }
  }

}

void CPlayer::SwingSword() {
    if (m_fSwingTimer <= 0.0f) {
        m_fSwingTimer = SWORD_SWING_DURATION;
        m_bSwordAttacked = false;
        m_pAudio->play(eSound::Gun);
    }
}

void CPlayer::SwingDagger() {
    if (m_fDaggerTimer <= 0.0f && m_fSwingTimer <= 0.0f) {
        m_fDaggerTimer = DAGGER_SWING_DURATION;
        m_bDaggerAttacked = false;
        m_pAudio->play(eSound::Gun);
    }
}

void CPlayer::SwingGreatSword() {
    if (m_fGreatSwordTimer <= 0.0f && m_fSwingTimer <= 0.0f && m_fDaggerTimer <= 0.0f) {
        m_fGreatSwordTimer = GREATSWORD_SWING_DURATION;
        m_bGreatSwordAttacked = false;
        m_pAudio->play(eSound::Gun);
    }
}

const Vector2 CPlayer::GetViewVector()const {
    if (m_bFacingLeft) {
        return Vector2(-1.0f, 0.0f);
    }
    else {
        return Vector2(1.0f, 0.0f);

    }
}

void CPlayer::SetFacingLeft(bool facingLeft) {
    m_bFacingLeft = facingLeft;
}

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
