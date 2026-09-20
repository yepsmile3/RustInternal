#include <Core/Hooks/Hooks.hpp>

auto Hooks::DoMovement( Projectile* _This, float deltaTime ) -> void
{
	if ( settings.BulletTracers )
	{
		auto currentVelocity = _This->currentVelocity( ) * deltaTime;
		auto magnitude = currentVelocity.magnitude( );
		float num = 1.f / magnitude;
		auto velocity = currentVelocity * num;
		auto arrowPosition = _This->currentPosition( ) + velocity * currentVelocity.magnitude( );
		auto traveledTime = _This->traveledTime( ) + deltaTime;
		bool flag = false;

		if ( auto hitTest = _This->hitTest( ) )
		{
			DDraw::Line( _This->currentPosition( ), hitTest->HitPointWorld( ), Color( 255, 255, 255 ), 60.f, true, true );
			DDraw::Sphere( hitTest->HitPointWorld( ), _This->thickness( ), Color( 255, 255, 255 ), 60.f, true );

			if ( hitTest->HitTransform( ) )
				DDraw::Text( L"x", hitTest->HitPointWorld( ), Color( 255, 0, 0 ), 60.f );

			if ( _This->DoRicochet( hitTest, hitTest->HitPointWorld( ), hitTest->HitPointWorld( ) ) || _This->DoHit( hitTest, hitTest->HitPointWorld( ), hitTest->HitPointWorld( ) ) )
				flag = true;

			if ( _This->isAlive( ) )
			{
				if ( flag && _This->traveledTime( ) < traveledTime )
					return Hooks::DoMovementhk.get_original< decltype( &DoMovement )>( )( _This, deltaTime );

				if ( _This->isAuthoritative( ) )
					DDraw::Arrow( _This->currentPosition( ), arrowPosition, _This->thickness( ), Color( 255, 255, 255 ), 60.f );
			}
		}
	
	}

	Hooks::DoMovementhk.get_original< decltype( &DoMovement )>( )( _This, deltaTime );
}