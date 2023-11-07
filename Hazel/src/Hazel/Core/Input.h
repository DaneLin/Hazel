#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Keycodes.h"
#include "Hazel/Core/MouseCodes.h"

namespace Hazel
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
	    static float GetMouseY();
	
	};
}
