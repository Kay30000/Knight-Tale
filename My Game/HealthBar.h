#include "Object.h"



class CHealthBar : public CObject {
protected:
	
	const UINT m_nMaxHealth = 12; ///< Maximum health.
	
	
	

public:
	CHealthBar(const Vector2& p); ///< Constructor.
	int currentFrame = 19;

}; //CHealthBar