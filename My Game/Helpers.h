/// \file Helpers.h
/// \brief Interface for helper functions.

#ifndef __L4RC_GAME_HELPERS_H__
#define __L4RC_GAME_HELPERS_H__

#include "GameDefines.h"

const Vector2 AngleToVector(const float theta); ///< Convert angle to vector.
const Vector2 VectorNormalCC(const Vector2& v); ///< Counterclockwise normal.
void NormalizeAngle(float& theta); ///< Normalize angle to \f$\pm\pi\f$.

inline float Dot(const Vector2& v1, const Vector2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline Vector2 Normalize(const Vector2& v) {
	const float lenSq = v.x * v.x + v.y * v.y;
	if (lenSq > 0.00001f) {
		const float invLen = 1.0f / sqrtf(lenSq);
		return Vector2(v.x * invLen, v.y * invLen);
	}
	return Vector2(0, 0);
}

#endif //__L4RC_GAME_HELPERS_H__