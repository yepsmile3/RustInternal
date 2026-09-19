#include "Features.hpp"


auto Features::RenderPlayer( BasePlayer* player ) -> void
{

}

auto Features::BulletTracers( ) -> void
{
	if ( Core::m_core->m_tracer.empty( ) )
		return;

	for ( int idx { 0 }; idx < Core::m_core->m_tracer.size( ); idx++ )
	{
		auto tracer = Core::m_core->m_tracer[ idx ];

		DDraw::Line( tracer.start, tracer.end, Color( 255, 255, 255, 100 ), 0.5f, true, false );
		DDraw::Sphere( tracer.end, 0.1f, Color( 255, 255, 255, 100 ), 0.5f, true );
	}
}

auto Features::InstantKill( ) -> void
{
	if ( !Core::m_core->heldEntity )
		return;

	if ( Core::m_core->heldEntity->lastShotTime( ) < 3.f )
		return;

	if ( !Core::m_core->Target )
		return;

	if ( !Core::m_core->Target->alive( ) )
		return;

	auto desyncTime = Time::realtimeSinceStartup( ) - Core::m_core->Local->lastSentTickTime( );
	auto desnycPercentage = ( ( desyncTime / 0.99f ) * 100.0f ) + 1.f;
	if( desnycPercentage > 90 )
	{
		Core::m_core->heldEntity->LaunchProjectile( );
		Core::m_core->heldEntity->ShotFired( );
		Core::m_core->heldEntity->UpdateAmmoDisplay( );
		Core::m_core->heldEntity->BeginCycle( );
	}
}

auto Features::FindManipulationAngle( ) -> void
{
	if ( !Core::m_core->Target || !Core::m_core->Local )
		return;
	
	if ( !Core::m_core->Target->alive( ) )
		return;

	auto targetTransform = Core::m_core->Target->BoneIndex( 48 );
	if ( !targetTransform )
		return;
	
	auto targetPosition = targetTransform->position( );
	if ( targetPosition.empty( ) )
		return;
	
	auto desnycTime = Time::realtimeSinceStartup( ) - !Core::m_core->Local->lastSentTickTime( );

	auto eyes = LocalPlayer::Entity( )->eyes( );
	if ( !memory::valid_pointer( eyes ) )
		return;

	auto bodyRight = eyes->BodyRight( );
	auto bodyFoward = eyes->BodyForward( );

	std::vector<Vector3> validAngles{ };


	for ( const auto& angle : validAngles )
	{    
		if ( !Core::m_core->LineCast( angle, targetPosition ) )
			continue;

		Core::m_core->manipulation_angle = angle;
	}


}

