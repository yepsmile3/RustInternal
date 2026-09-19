#pragma once
#include <Core/Core.hpp>

namespace input
{
	inline bool is_pressed( int key )
	{
		return GetAsyncKeyState( key );
	}
}