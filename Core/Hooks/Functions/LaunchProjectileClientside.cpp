#include <Core/Hooks/Hooks.hpp>

auto Hooks::LaunchProjectileClientside( BaseProjectile* _This, ItemDefinition* ammo, int projectileCount, float projSpreadaimCone ) -> void
{
	if ( !Core::m_core->Local || !Core::m_core->Target || !Core::m_core->Target->alive( ) || Core::m_core->manipulation_angle.empty( ) )
		return Hooks::LaunchProjectileClientsidehk.get_original< decltype( &LaunchProjectileClientside )>( )( _This, ammo, projectileCount, projSpreadaimCone );

	auto eyes = Core::m_core->Local->eyes( );
	if ( !eyes )
		return Hooks::LaunchProjectileClientsidehk.get_original< decltype( &LaunchProjectileClientside )>( )( _This, ammo, projectileCount, projSpreadaimCone );

	static auto viewOffset = eyes->viewOffset( );

	if ( settings.Manipulation && input::is_pressed( VK_XBUTTON1 ) )
		eyes->viewOffset( ) = Core::m_core->manipulation_angle;

	Hooks::LaunchProjectileClientsidehk.get_original< decltype( &LaunchProjectileClientside )>( )( _This, ammo, projectileCount, projSpreadaimCone );

	eyes->viewOffset( ) = viewOffset;
}