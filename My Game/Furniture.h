#include "Object.h"

class CFurniture : public CObject {
protected:
    const UINT m_nMaxHealth = 99; ///< Maximum health.
    UINT m_nHealth = m_nMaxHealth; ///< Current health.

    
    
    

public:
    CFurniture(const Vector2& p); ///< Constructor.
   
}; //CBullet