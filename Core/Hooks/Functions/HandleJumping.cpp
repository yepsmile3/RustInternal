#include <Core/Hooks/Hooks.hpp>


auto Hooks::HandleJumping( PlayerWalkMovement* _This, ModelState* state, bool wantsJump, bool jumpInDirection ) -> void
{
	if ( settings.BunnyHop && Input::GetKey( KeyCode::Space ) )	
		wantsJump = true;

	Hooks::HandleJumpinghk.get_original< decltype( &HandleJumping )>( )( _This, state, wantsJump, jumpInDirection );
} 