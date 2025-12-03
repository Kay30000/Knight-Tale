/// \file TileManager.h
/// \brief Interface for the tile manager CTileManager.

#ifndef __L4RC_GAME_TILEMANAGER_H__
#define __L4RC_GAME_TILEMANAGER_H__

#include <vector>

#include "Common.h"
#include "Settings.h"
#include "Sprite.h"
#include "GameDefines.h"

/// \brief The tile manager.
///
/// The tile manager is responsible for the tile-based background.

class CTileManager: 
  public CCommon, 
  public LSettings
{
    

  private:
    size_t m_nWidth = 0; ///< Number of tiles wide.
    size_t m_nHeight = 0; ///< Number of tiles high.

    float m_fTileSize = 0.0f; ///< Tile width and height.

    char** m_chMap = nullptr; ///< The level map.

    std::vector<BoundingBox> m_vecWalls; ///< AABBs for the walls.
    std::vector<Vector2> m_vecTurrets;///< Turret positions.
    std::vector<Vector2> m_vecStationaryTurrets;
    
 
    Vector2 m_vPlayer; ///< Player location.

    void MakeBoundingBoxes(); ///< Make bounding boxes for walls.

  public:
    CTileManager(size_t); ///< Constructor.
    ~CTileManager(); ///< Destructor.

    struct furniture {
        Vector2 location;
        char type;
    
    };

    std::vector<Vector2> m_vecZombies;
    std::vector<furniture> m_vecFurniture;

    void LoadMapFromImageFile(char*); ///< Load map.
    void LoadMap(char*); ///< Load a map.
    void Draw(eSprite); ///< Draw the map with a given tile.
    void DrawBoundingBoxes(eSprite); ///< Draw the bounding boxes.

    const std::vector<Vector2>& GetTurrets() const { return m_vecTurrets; }
    const std::vector<furniture>& GetFurniture() const { return m_vecFurniture; }
    const Vector2& GetPlayerPos() const { return m_vPlayer; }
    const std::vector<Vector2>& GetStationaryTurrets() const { return m_vecStationaryTurrets; }
    
    

    void GetObjects(std::vector<Vector2>& turrets, std::vector<furniture>& furniture, Vector2& player, std::vector<Vector2>& zombies) const; ///< Get objects.

    const bool Visible(const Vector2&, const Vector2&, float) const; ///< Check visibility.
    const bool CollideWithWall(BoundingSphere, Vector2&, float&) const; ///< Object-wall collision test.
}; //CTileManager

#endif //__L4RC_GAME_TILEMANAGER_H__