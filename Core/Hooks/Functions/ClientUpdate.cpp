#include <Core/Hooks/Hooks.hpp>

auto Hooks::ClientUpdate( BasePlayer* _This ) -> void
{
	Hooks::ClientUpdatehk.get_original< decltype( &ClientUpdate )>( )( _This );
}