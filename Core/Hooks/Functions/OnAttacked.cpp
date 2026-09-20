#include <Core/Hooks/Hooks.hpp>


auto Hooks::OnAttacked( BaseCombatEntity* _This, HitInfo* hitInfo ) -> void
{
	Core::Tracer tracer;
	tracer.start = hitInfo->PointStart( );
	tracer.end = hitInfo->PointEnd( );
	Core::m_core->m_tracer.emplace_back( tracer );

	Hooks::OnAttackedhk.get_original< decltype( &OnAttacked )>( )( _This, hitInfo );
}