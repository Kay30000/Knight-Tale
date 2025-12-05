/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"
#include "TileManager.h"
#include "Turret.h"
#include "HealthBar.h"

#include "shellapi.h"

/// Delete the renderer, the object manager, and the tile manager. The renderer
/// needs to be deleted before this destructor runs so it will be done elsewhere.

CGame::~CGame(){
  delete m_pParticleEngine;
  delete m_pObjectManager;
  delete m_pTileManager;
} //destructor

/// Initialize the renderer, the tile manager and the object manager, load 
/// images and sounds, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new LSpriteRenderer(eSpriteMode::Batched2D); 
  m_pRenderer->Initialize(eSprite::Size); 
  LoadImages(); //load images from xml file list
  
  m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::Tile));
  m_pObjectManager = new CObjectManager; //set up the object manager 
  LoadSounds(); //load the sounds for this game

  m_pParticleEngine = new LParticleEngine2D(m_pRenderer);

  BeginGame();
} //Initialize

/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CGame::LoadImages(){  
  m_pRenderer->BeginResourceUpload();

  m_pRenderer->Load(eSprite::Pickup, "pickup");
  m_pRenderer->Load(eSprite::Tile,    "tile"); 
  m_pRenderer->Load(eSprite::Bullet,  "bullet");
  m_pRenderer->Load(eSprite::Bullet2, "bullet2");
  m_pRenderer->Load(eSprite::Smoke,   "smoke");
  m_pRenderer->Load(eSprite::Spark,   "spark");
  m_pRenderer->Load(eSprite::Turret,  "turret");
  m_pRenderer->Load(eSprite::Line,    "greenline");
  m_pRenderer->Load(eSprite::Furniture, "furniture");
  m_pRenderer->Load(eSprite::Fireball, "fireball");
  m_pRenderer->Load(eSprite::sword, "sword");
  m_pRenderer->Load(eSprite::greatsword, "greatsword");
  m_pRenderer->Load(eSprite::dagger, "dagger");
  m_pRenderer->Load(eSprite::shield, "shield");
  m_pRenderer->Load(eSprite::PlayerStandRight, "standright");
  m_pRenderer->Load(eSprite::PlayerStandLeft, "standleft");
  m_pRenderer->Load(eSprite::PlayerStandRight, "standright");
  m_pRenderer->Load(eSprite::PlayerStandUp, "standup");
  m_pRenderer->Load(eSprite::PlayerStandDown, "standdown");
  m_pRenderer->Load(eSprite::PlayerWalkLeftSpriteSheet, "walkleftsheet");
  m_pRenderer->Load(eSprite::PlayerWalkLeft, "walkleft");
  m_pRenderer->Load(eSprite::PlayerWalkRightSpriteSheet, "walkrightsheet");
  m_pRenderer->Load(eSprite::PlayerWalkRight, "walkright");
  m_pRenderer->Load(eSprite::PlayerWalkUpSpriteSheet, "walkupsheet");
  m_pRenderer->Load(eSprite::PlayerWalkUp, "walkup");
  m_pRenderer->Load(eSprite::PlayerWalkDownSpriteSheet, "walkdownsheet");
  m_pRenderer->Load(eSprite::PlayerWalkDown, "walkdown");

  m_pRenderer->Load(eSprite::PlayerAttackLeftSpriteSheet, "attackleftsheet");
  m_pRenderer->Load(eSprite::PlayerAttackLeft, "attackleft");
  m_pRenderer->Load(eSprite::PlayerAttackRightSpriteSheet, "attackrightsheet");
  m_pRenderer->Load(eSprite::PlayerAttackRight, "attackright");
  m_pRenderer->Load(eSprite::PlayerAttackUpSpriteSheet, "attackupsheet");
  m_pRenderer->Load(eSprite::PlayerAttackUp, "attackup");
  m_pRenderer->Load(eSprite::PlayerAttackDownSpriteSheet, "attackdownsheet");
  m_pRenderer->Load(eSprite::PlayerAttackDown, "attackdown");

  m_pRenderer->Load(eSprite::HealthBar, "healthbar");
  m_pRenderer->Load(eSprite::HealthBar, "healthbar");
  

  m_pRenderer->EndResourceUpload();
} //LoadImages

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);

  m_pAudio->Load(eSound::Grunt, "grunt");
  m_pAudio->Load(eSound::Clang, "clang");
  m_pAudio->Load(eSound::Gun, "gun");
  m_pAudio->Load(eSound::Ricochet, "ricochet");
  m_pAudio->Load(eSound::Start, "start");
  m_pAudio->Load(eSound::Boom, "boom");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Ask the object manager to create a player object and turrets specified by
/// the tile manager.

void CGame::CreateObjects() {
    std::vector<Vector2> turretpos; //vector of turret positions
    std::vector<CTileManager::furniture> furniturepos; //vector of furniture positions
    Vector2 playerpos; //player positions


    m_pTileManager->GetObjects(turretpos, furniturepos, playerpos); //get positions


    for (const Vector2& pos : turretpos) {
        CTurret* pTurret = (CTurret*)m_pObjectManager->create(eSprite::Turret, pos);


        std::vector<Vector2> patrolPath = {
        pos,
        pos + Vector2(100.0f, 0.0f)
        };
        pTurret->InitializePatrol(patrolPath);
    }


    for (CTileManager::furniture furn : furniturepos)
    {
        Vector2 pos = furn.location;
        m_pObjectManager->createFurniture(eSprite::Furniture, pos, furn.type);
    }


    m_pPlayer = (CPlayer*)m_pObjectManager->create(eSprite::PlayerStandDown, playerpos);


    if (m_pPlayer) {
        m_pPlayer->Stop();
    }
} //CreateObjects

/*
void CGame::CreateObjects(){
  std::vector<Vector2> turretpos; //vector of turret positions
  std::vector<CTileManager::furniture> furniturepos; //vector of furniture positions
  Vector2 playerpos; //player positions

  m_pTileManager->GetObjects(turretpos, furniturepos, playerpos); //get positions

  
  

  for(const Vector2& pos: turretpos)
    m_pObjectManager->create(eSprite::Turret, pos);
 
  for (const Vector2& pos : turretpos) {
      auto pTurret = (CTurret*)m_pObjectManager->create(eSprite::Turret, pos);


      std::vector<Vector2> patrolPath = {
      pos,
      pos + Vector2(100.0f, 0.0f)
      };
      pTurret->InitializePatrol(patrolPath);
  }

  for (CTileManager::furniture furn : furniturepos)
  {
	  Vector2 pos = furn.location;
      m_pObjectManager->createFurniture(eSprite::Furniture, pos, furn.type);
  }

  m_pPlayer = (CPlayer*)m_pObjectManager->create(eSprite::PlayerStandDown, playerpos);

  if (m_pPlayer) {
      m_pPlayer->Stop();
  }

} //CreateObjects
*/
/// Call this function to start a new game. This should be re-entrant so that
/// you can restart a new game without having to shut down and restart the
/// program. Clear the particle engine to get rid of any existing particles,
/// delete any old objects out of the object manager and create some new ones.

void CGame::BeginGame(){  
  m_pParticleEngine->clear(); //clear old particles
  
  switch(m_nNextLevel){
    //case 0: m_pTileManager->LoadMap("Media\\Maps\\tiny.txt"); break;
    //case 1: m_pTileManager->LoadMap("Media\\Maps\\small.txt"); break;
    case 0: m_pTileManager->LoadMap("Media\\Maps\\map.txt"); break;
    //case 0: m_pTileManager->LoadMapFromImageFile("Media\\Maps\\maze.png");break;
  } //switch

  m_pObjectManager->clear(); //clear old objects
  CreateObjects(); //create new objects (must be after map is loaded) 
  m_pAudio->stop(); //stop all  currently playing sounds
  m_pAudio->play(eSound::Start); //play start-of-game sound
  m_eGameState = eGameState::Playing; //now playing
} //BeginGame

/// Poll the keyboard state and respond to the key presses that happened since
/// the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState();
  if (m_pKeyboard->TriggerDown(VK_RETURN)) {
      m_nNextLevel = (m_nNextLevel + 1) % 4;
      BeginGame();
  } 
  
  
    
  
  if(m_pKeyboard->TriggerDown(VK_F1)) //help
    ShellExecute(0, 0, "https://larc.unt.edu/code/topdown/", 0, 0, SW_SHOW);
  
  if(m_pKeyboard->TriggerDown(VK_F2)) //toggle frame rate
    m_bDrawFrameRate = !m_bDrawFrameRate;
  
  if(m_pKeyboard->TriggerDown(VK_F3)) //toggle AABB drawing
    m_bDrawAABBs = !m_bDrawAABBs; 

  if (m_pKeyboard->TriggerDown(VK_F4)) // move to next level
  {
      m_nNextLevel = (m_nNextLevel + 1) % 4;
      BeginGame();
  }



  if(m_pKeyboard->TriggerDown(VK_BACK)) //start game
    BeginGame();

  if (m_pKeyboard->TriggerDown('P') || m_pKeyboard->TriggerDown(VK_ESCAPE)) {
      m_eGameState = (m_eGameState == eGameState::Paused) ?
          eGameState::Playing :
          eGameState::Paused;
  }

  if (m_pKeyboard->TriggerDown('O'))
  {
	  Vector2 pos = m_pPlayer->m_vPos + Vector2(50.0f, 0.0f);
      m_pObjectManager->SpawnPickup(pos, 1);
  }

  if (m_eGameState != eGameState::Paused && m_pPlayer) {
      int playerWeapon = m_pPlayer->weaponEquipped;
      m_pPlayer->SetRotSpeed(0.0f);

      float fTargetSpeed = 0.0f;

      bool bMovementKeyHeld = m_pKeyboard->Down('W') || m_pKeyboard->Down('S') || m_pKeyboard->Down('A') || m_pKeyboard->Down('D');


     

      m_pPlayer->SetSpeed(0.0f);

      // Vertical Movement
      if (m_pKeyboard->Down('W') && !m_pKeyboard->Down('S')) {
          fTargetSpeed = PLAYER_NORMAL_SPEED; 
          m_pPlayer->WalkUp();
      }
      else if (m_pKeyboard->Down('S') && !m_pKeyboard->Down('W')) {
          fTargetSpeed = -PLAYER_NORMAL_SPEED;
          m_pPlayer->StrafeBack();
          m_pPlayer->WalkDown();
      }

      // Horizontal Movement 
      else if (!bMovementKeyHeld || m_pKeyboard->Down('A') || m_pKeyboard->Down('D')) {
          if (m_pKeyboard->Down('D') && !m_pKeyboard->Down('A')) {
              fTargetSpeed = PLAYER_NORMAL_SPEED;
              m_pPlayer->StrafeRight();
              m_pPlayer->WalkRight();
          }
          else if (m_pKeyboard->Down('A') && !m_pKeyboard->Down('D')) {
              fTargetSpeed = -PLAYER_NORMAL_SPEED;
              m_pPlayer->StrafeLeft();
              m_pPlayer->WalkLeft();
          }
      }
      if (!bMovementKeyHeld) {
          m_pPlayer->Stop();
      }

      // Weapons

      // Arrow
      if (m_pKeyboard->Down('I') && m_pKeyboard->Down('L') && !m_pKeyboard->Down('K') && !m_pKeyboard->Down('J'))
      {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(.707, .707);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
		  }
      }
      else if (m_pKeyboard->Down('I') && m_pKeyboard->Down('J') && !m_pKeyboard->Down('K') && !m_pKeyboard->Down('L'))
      {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(-.707, .707);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
          }
      }
      else if (m_pKeyboard->Down('J') && m_pKeyboard->Down('K') && !m_pKeyboard->Down('I') && !m_pKeyboard->Down('L'))
      {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(-.707, -.707);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
          }
      }
      else if (m_pKeyboard->Down('K') && m_pKeyboard->Down('L') && !m_pKeyboard->Down('I') && !m_pKeyboard->Down('J'))
      {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(.707, -.707);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
          }
      }
      else if (m_pKeyboard->Down('L')) {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(1,0);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
          }
      }
      else if (m_pKeyboard->Down('J')) {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(-1, 0);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
          }
      }
      else if (m_pKeyboard->Down('I')) {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(0, 1);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
          }
      }
      else if (m_pKeyboard->Down('K')) {
          if (m_pPlayer->m_pBulletCooldown->Triggered()) {
              Vector2 vDir(0, -1);
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir, playerWeapon);
          }
      }

      /*
      // Fireball
      if (m_pKeyboard->TriggerDown('Q')) {
          if (m_pPlayer->m_pFireballCooldown->Triggered()) {
              Vector2 vDir = m_pPlayer->GetDirectionVector();
              m_pObjectManager->FireGun(m_pPlayer, eSprite::Fireball, vDir);
          }
      }

      // Regular Sword
      if (m_pKeyboard->TriggerDown('E')) {
          if (m_pPlayer->m_pSwordCooldown->Triggered()) {
              Vector2 vDir = m_pPlayer->GetDirectionVector();
              m_pObjectManager->FireGun(m_pPlayer, eSprite::sword, vDir);
          }
      }

      // Great Sword
      if (m_pKeyboard->TriggerDown('R')) {
          if (m_pPlayer->m_pGreatswordCooldown->Triggered()) {
              Vector2 vDir = m_pPlayer->GetDirectionVector();
              m_pObjectManager->FireGun(m_pPlayer, eSprite::greatsword, vDir);
          }
      }

      // Dagger
      if (m_pKeyboard->TriggerDown('T')) {
          if (m_pPlayer->m_pDaggerCooldown->Triggered()) {
              Vector2 vDir = m_pPlayer->GetDirectionVector();
              m_pObjectManager->FireGun(m_pPlayer, eSprite::dagger, vDir);
          }
      }

      */



      // Shield
      if (m_pKeyboard->Down(VK_LSHIFT)) {
          if (!m_pPlayer->m_bShieldActive) {
              m_pPlayer->m_bShieldActive = true;

              Vector2 playerDir = m_pPlayer->GetDirectionVector();
              Vector2 shieldPos = m_pPlayer->m_vPos + playerDir * SHIELD_OFFSET;
              m_pPlayer->m_pShieldObject = m_pObjectManager->create(eSprite::shield, shieldPos);
              m_pPlayer->m_pShieldObject->SetStatic(true);
              m_pPlayer->m_pShieldObject->m_fRoll = m_pPlayer->m_fRoll;
          }

          if (bMovementKeyHeld) {

              if (fTargetSpeed > 0.0f) {
                  fTargetSpeed = PLAYER_SHIELD_SPEED;
              }
              else if (fTargetSpeed < 0.0f) {
                  fTargetSpeed = -PLAYER_SHIELD_SPEED;
              }
          }
      }
      else {
          if (m_pPlayer->m_bShieldActive) {
              m_pPlayer->m_bShieldActive = false;

              if (m_pPlayer->m_pShieldObject) {
                  m_pPlayer->m_pShieldObject->SetDead();
                  m_pPlayer->m_pShieldObject = nullptr;
              }
          }
      }

      m_pPlayer->SetSpeed(fTargetSpeed);



    if(m_pKeyboard->TriggerDown('G')) //toggle god mode
      m_bGodMode = !m_bGodMode;
  } 
} 

/// Poll the XBox controller state and respond to the controls there.

void CGame::ControllerHandler(){
  if(!m_pController->IsConnected())return;

  m_pController->GetState(); //get state of controller's controls 
  
  if(m_pPlayer){ //safety
    m_pPlayer->SetSpeed(100*m_pController->GetRTrigger());
    m_pPlayer->SetRotSpeed(-2.0f*m_pController->GetRThumb().x);

    //if(m_pController->GetButtonRSToggle()) //fire gun
     //   m_pObjectManager->FireGun(m_pPlayer, eSprite::Bullet, vDir); // Pass vDir

    if(m_pController->GetDPadRight()) //strafe right
      m_pPlayer->StrafeRight();
  
    if(m_pController->GetDPadLeft()) //strafe left
      m_pPlayer->StrafeLeft();

    if(m_pController->GetDPadDown()) //strafe back
      m_pPlayer->StrafeBack();
  } //if
} //ControllerHandler

/// Draw the current frame rate to a hard-coded position in the window.
/// The frame rate will be drawn in a hard-coded position using the font
/// specified in `gamesettings.xml`.

void CGame::DrawFrameRateText(){
  const std::string s = std::to_string(m_pTimer->GetFPS()) + " fps"; //frame rate
  const Vector2 pos(m_nWinWidth - 128.0f, 30.0f); //hard-coded position
  m_pRenderer->DrawScreenText(s.c_str(), pos); //draw to screen
} //DrawFrameRateText

/// Draw the god mode text to a hard-coded position in the window using the
/// font specified in `gamesettings.xml`.

void CGame::DrawPausedText() {
    const Vector2 pos(m_nWinWidth / 2.0f - 64.0f, m_nWinHeight / 2.0f);
    m_pRenderer->DrawScreenText("Paused", pos);
}

void CGame::DrawGodModeText(){
  const Vector2 pos(64.0f, 30.0f); //hard-coded position
  m_pRenderer->DrawScreenText("God Mode", pos); //draw to screen
} //DrawGodModeText

/// Ask the object manager to draw the game objects. The renderer is notified of
/// the start and end of the frame so that it can let Direct3D do its
/// pipelining jiggery-pokery.

void CGame::RenderFrame(){
  m_pRenderer->BeginFrame(); //required before rendering

  m_pObjectManager->draw(); //draw objects
  m_pParticleEngine->Draw(); //draw particles
  if(m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required
  if(m_bGodMode)DrawGodModeText(); //draw god mode text, if required
  if (m_eGameState == eGameState::Paused) //draw paused text
      DrawPausedText();

  m_pRenderer->EndFrame(); //required after rendering
} //RenderFrame

/// Make the camera follow the player, but don't let it get too close to the
/// edge unless the world is smaller than the window, in which case we just
/// center everything.

void CGame::FollowCamera(){
  if(m_pPlayer == nullptr)return; //safety

  Vector3 vCameraPos(m_pPlayer->GetPos()); //player position

  if(m_vWorldSize.x > m_nWinWidth){ //world wider than screen
    vCameraPos.x = std::max(vCameraPos.x, m_nWinWidth/2.0f); //stay away from the left edge
    vCameraPos.x = std::min(vCameraPos.x, m_vWorldSize.x - m_nWinWidth/2.0f);  //stay away from the right edge
  } //if
  else vCameraPos.x = m_vWorldSize.x/2.0f; //center horizontally.
  
  if(m_vWorldSize.y > m_nWinHeight){ //world higher than screen
    vCameraPos.y = std::max(vCameraPos.y, m_nWinHeight/2.0f);  //stay away from the bottom edge
    vCameraPos.y = std::min(vCameraPos.y, m_vWorldSize.y - m_nWinHeight/2.0f); //stay away from the top edge
  } //if
  else vCameraPos.y = m_vWorldSize.y/2.0f; //center vertically

  m_pRenderer->SetCameraPos(vCameraPos); //camera to player
} //FollowCamera

/// This function will be called regularly to process and render a frame
/// of animation, which involves the following. Handle keyboard input.
/// Notify the audio player at the start of each frame so that it can prevent
/// multiple copies of a sound from starting on the same frame.  
/// Move the game objects. Render a frame of animation. 

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  if (m_eGameState == eGameState::Paused) {
      RenderFrame();
      return;
  }
  ControllerHandler(); //handle controller input
  m_pAudio->BeginFrame(); //notify audio player that frame has begun
  
  m_pTimer->Tick([&](){ //all time-dependent function calls should go here
    m_pObjectManager->move(); //move all objects
    FollowCamera(); //make camera follow player
    m_pParticleEngine->step(); //advance particle animation
  });

  RenderFrame(); //render a frame of animation
  ProcessGameState(); //check for end of game
} //ProcessFrame

/// Take action appropriate to the current game state. If the game is currently
/// playing, then if the player has been killed or all turrets have been
/// killed, then enter the wait state. If the game has been in the wait
/// state for longer than 3 seconds, then restart the game.

void CGame::ProcessGameState(){
  static float t = 0; //time at start of game

  switch(m_eGameState){
    case eGameState::Playing:
      if(m_pPlayer == nullptr || m_pObjectManager->GetNumTurrets() == 0){
        m_eGameState = eGameState::Waiting; //now waiting
        t = m_pTimer->GetTime(); //start wait timer
      } //if
      break;

    case eGameState::Waiting:
      if(m_pTimer->GetTime() - t > 3.0f){ //3 seconds has elapsed since level end
        if(m_pObjectManager->GetNumTurrets() == 0) //player won
            m_nNextLevel = (m_nNextLevel + 1) % 4; //note: 4 instead of 3
        BeginGame(); //restart game
      } //if
      break;
  } //switch
} //CheckForEndOfGame