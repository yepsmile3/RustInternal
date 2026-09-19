#include <Core/Hooks/Hooks.hpp>


auto Hooks::PSilent( float aimCone, Vector3 direction, bool anywhereInside ) -> Vector3
{
	if ( !Core::m_core->Target || !Core::m_core->Target->alive( ) || !Core::m_core->heldEntity )
		return Hooks::PSilenthk.get_original< decltype( &PSilent )>( )( aimCone, direction, anywhereInside );

	if ( settings.PSilent ) {

		auto targetTransform = Core::m_core->Target->BoneIndex( 48 );
		if( !targetTransform )
			return Hooks::PSilenthk.get_original< decltype( &PSilent )>( )( aimCone, direction, anywhereInside );

		auto targetPos = targetTransform->position( );
		if ( targetPos.empty( ) )
			return Hooks::PSilenthk.get_original< decltype( &PSilent )>( )( aimCone, direction, anywhereInside );

		direction = ( targetPos - Core::m_core->Local->eyes( )->position( ) ).normalized( );
	}

	aimCone = 0.f;
	
	return Hooks::PSilenthk.get_original< decltype( &PSilent )>( )( aimCone, direction, anywhereInside );
}