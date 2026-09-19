#include <Core/Hooks/Hooks.hpp>

auto Hooks::Play( void* _This, String name ) -> void
{
	if ( settings.NoShootAnim ) {
		if ( name.Contains( L"attack" ) || name.Contains( L"dryfire" ) || name.Contains( L"reload" ) || name.Contains( L"manualcycle" ) )
			return;
	}

	Hooks::Playhk.get_original< decltype( &Play )>( )( _This, name );
}