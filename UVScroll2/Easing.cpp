#include "pch.h"
#include "Easing.h"

namespace Easing {
	float Linear(float a, float b, float t)
	{
		return (1.0f - t) * a + t * b;
	}
}

