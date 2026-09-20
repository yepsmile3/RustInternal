#include <Core/Hooks/Hooks.hpp>


auto Hooks::Run( void* _This, String name, Vector3 a, Vector3 b, Vector3 c ) -> void
{
	if ( name.Contains( L"jump" ) || name.Contains( L"hit_notify" ) || name.Contains( L"headshot" ) || name.Contains( L"screen_land" ) )
		return;

	Hooks::Runhk.get_original< decltype( &Run )>( )( _This, name, a, b, c );
}