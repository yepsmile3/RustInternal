#include <Core/Hooks/Hooks.hpp>

auto Hooks::SendProjectileAttack( BasePlayer* _This, ProtoBuf::PlayerProjectileAttack* attack ) -> void
{

	if ( settings.BoneOverride )
	{
		auto playerAttack = attack->playerAttack( );
		if ( !playerAttack )
			return Hooks::SendProjectileAttackhk.get_original< decltype( &SendProjectileAttack )>( )( _This, attack );

		auto __attack = playerAttack->attack( );
		if ( !__attack )
			return Hooks::SendProjectileAttackhk.get_original< decltype( &SendProjectileAttack )>( )( _This, attack );

		__attack->hitBone( ) = 48;
	}

	Hooks::SendProjectileAttackhk.get_original< decltype( &SendProjectileAttack )>( )( _This, attack );

}