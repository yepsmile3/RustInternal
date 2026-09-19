#include <Core/Hooks/Hooks.hpp>

auto Hooks::PerformanceUI( void* _This ) -> void
{
	Hooks::PerformanceUIhk.get_original< decltype( &PerformanceUI )>( )( _This );


	//Core::m_core->Cache( );
}