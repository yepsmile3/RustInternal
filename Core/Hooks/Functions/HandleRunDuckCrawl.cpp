#include <Core/Hooks/Hooks.hpp>


auto Hooks::HandleRunDuckCrawl( PlayerWalkMovement* _This, ModelState* state, bool wantsRun, bool wantsDuck, bool wantsCrawl ) -> void
{
	if ( settings.OmniSprint )
		wantsRun = true;

	Hooks::HandleRunDuckCrawlhk.get_original<decltype( &HandleRunDuckCrawl )>( )( _This, state, wantsRun, wantsDuck, wantsCrawl );
}