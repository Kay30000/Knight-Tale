#include "Object.h"



class CHealthBar : public CObject {
protected:
	
	const UINT m_nMaxHealth = 12; ///< Maximum health.
	UINT m_nHealth = m_nMaxHealth; ///< Current health.
	
	

public:
	CHealthBar(const Vector2& p, int maxHealth); ///< Constructor.
	virtual void move(int c); 
	int playerHealth;
	int maxHealth;
	int getFrame();
	int currentFrame = 19;

}; //CHealthBar