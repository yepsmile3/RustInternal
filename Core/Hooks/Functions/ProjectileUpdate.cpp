#include <Core/Hooks/Hooks.hpp>


auto Hooks::ProjectileUpdate( Projectile* _This ) -> void
{

	if ( settings.BulletTp )
	{
		if( !Core::m_core->Target )
			return Hooks::ProjectileUpdatehk.get_original< decltype( &ProjectileUpdate )>( )( _This );
		
		if( !Core::m_core->Target->alive( ) )
			return Hooks::ProjectileUpdatehk.get_original< decltype( &ProjectileUpdate )>( )( _This );

		auto target_pos = Core::m_core->Target->BoneIndex( 48 )->position( );
		if ( target_pos.empty( ) )
			return Hooks::ProjectileUpdatehk.get_original< decltype( &ProjectileUpdate )>( )( _This );

		if ( target_pos.distance_3d( _This->currentPosition( ) ) < 2.f )
		{
			auto distance = target_pos.distance_3d( _This->currentPosition( ) );

			auto new_position = _This->currentPosition( ) + Vector3( 0, 0, distance );

			_This->transform( )->set_position( new_position );

			if ( target_pos.distance_3d( new_position ) > 2.f )
				return Hooks::ProjectileUpdatehk.get_original< decltype( &ProjectileUpdate )>( )( _This );
		}
	}

	Hooks::ProjectileUpdatehk.get_original< decltype( &ProjectileUpdate )>( )( _This );
}