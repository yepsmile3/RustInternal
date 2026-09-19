#include "Core.hpp"

auto Core::OnFrame( ) -> void 
{
	if ( !Core::m_core->Local || !Core::m_core->cache_finished )
		return;

	if ( settings.Manipulation && Input::GetKey( KeyCode::Mouse3 ) )
	{
		auto screenCenter = ImVec2( Screen::width( ) / 2, Screen::height( ) / 2 );
		auto desyncTime = Time::realtimeSinceStartup( ) - Core::m_core->Local->lastSentTickTime( );
	}
}

auto Core::GetBounds( BasePlayer* player ) -> Bounds
{
	
}

auto Core::CacheBounds( BasePlayer* player ) -> void
{
	
}

auto Core::Cache( ) -> void
{

}

auto Core::World2Screen( Vector3 world ) -> Vector3
{
	static auto camera = Camera::main( );
	if ( !camera )
		return Vector3( 0, 0, 0 );

	auto camera_ = *reinterpret_cast< uintptr_t* >( camera + 0x10 );
	if ( !camera_ )
		return Vector3( 0, 0, 0 );

	auto matrix = *reinterpret_cast< Matrix4x4* >( camera_ + 0x2e4 );
	if ( !matrix.m )
		return Vector3( 0, 0, 0 );

	Vector3 out;
	const auto temp = matrix.transpose( );

	auto trans = Vector3 { temp[ 3 ][ 0 ], temp[ 3 ][ 1 ], temp[ 3 ][ 2 ] };
	auto up = Vector3 { temp[ 1 ][ 0 ], temp[ 1 ][ 1 ], temp[ 1 ][ 2 ] };
	auto right = Vector3 { temp[ 0 ][ 0 ], temp[ 0 ][ 1 ], temp[ 0 ][ 2 ] };

	float w = trans.dot( world ) + temp[ 3 ][ 3 ];

	if ( w < 0.001f ) {
		return Vector3( 0, 0, 0 );
	}

	float x = right.dot( world ) + temp[ 0 ][ 3 ];
	float y = up.dot( world ) + temp[ 1 ][ 3 ];

	out.x = ( static_cast< float >( Screen::width( ) ) / 2 ) * ( 1 + x / w );
	out.y = ( static_cast< float >( Screen::height( ) ) / 2 ) * ( 1 - y / w );
	out.z = 0.0f;

	return out;
}

auto Core::CalculateFov( Vector3 position ) -> float
{
	auto screen_pos = World2Screen( position );
	if ( screen_pos.empty( ) )
		return 10000;

	auto get_2d_dist = [ & ] ( const Vector2& source, const Vector3& dest ) {
		return Vector3::sqrtf( ( source.x - dest.x ) * ( source.x - dest.x ) + ( source.y - dest.y ) * ( source.y - dest.y ) );
	};

	return get_2d_dist( Vector2( Screen::width( ) / 2, Screen::height( ) / 2 ), screen_pos );
}

auto Core::LineCast( Vector3 a, Vector3 b ) -> bool
{
	RaycastHit hit;
	return Physics::Linecast( a, b, &hit, 10551296 );
}
