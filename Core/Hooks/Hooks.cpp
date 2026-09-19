#include "Hooks.hpp"

auto Hooks::Start( ) -> void
{
	const auto SwapChainFn = reinterpret_cast< IDXGISwapChain * ( * )( ) >( pattern::find( L"UnityPlayer.dll", "48 83 ec 28 e8 ? ? ? ? 48 8b 80 a0 03 00 00" ) )( );
	if ( !SwapChainFn )
		return;
	static auto SwapChain = *reinterpret_cast< PVOID** > ( SwapChainFn );
	static auto PresentAddr = reinterpret_cast< HRESULT( __fastcall* )( IDXGISwapChain*, UINT, UINT ) >( SwapChain[ 8 ] );
	MH_CreateHook( PresentAddr, Present::PresentHk, &Present::o_present );
	MH_EnableHook( PresentAddr );

	Hooks::DoMovementhk.InitializeHook( "", "Projectile", "DoMovement", Hooks::DoMovement );
	Hooks::PerformanceUIhk.InitializeHook( "Facepunch", "PerformanceUI", "Update", Hooks::PerformanceUI );
	Hooks::ClientUpdatehk.InitializeHook( "", "BasePlayer", "ClientUpdate", Hooks::ClientUpdate );
	Hooks::HandleRunDuckCrawlhk.InitializeHook( "", "PlayerWalkMovement", "HandleRunDuckCrawl", Hooks::HandleRunDuckCrawl );
	Hooks::ClientInputhk.InitializeHook( "", "BasePlayer", "ClientInput", Hooks::ClientInput );
	Hooks::OnAttackedhk.InitializeHook( "", "BaseCombatEntity", "OnAttacked", Hooks::OnAttacked );
	Hooks::HandleJumpinghk.InitializeHook( "", "PlayerWalkMovement", "HandleJumping", Hooks::HandleJumping );
	Hooks::PSilenthk.InitializeHook( "", "AimConeUtil", "GetModifiedAimConeDirection", Hooks::PSilent );
	Hooks::ProjectileUpdatehk.InitializeHook( "", "Projectile", "Update", Hooks::ProjectileUpdate );
	Hooks::OnDisconnectedhk.InitializeHook( "Network", "Connection", "OnDisconnected", Hooks::OnDisconnected );
	Hooks::SendProjectileAttackhk.InitializeHook( "", "BasePlayer", "SendProjectileAttack", Hooks::SendProjectileAttack );
	Hooks::LaunchProjectileClientsidehk.InitializeHook( "", "BaseProjectile", "LaunchProjectileClientside", Hooks::LaunchProjectileClientside );
	Hooks::Playhk.InitializeHook( "", "ViewModel", "Play", Hooks::Play );
	Hooks::Runhk.InitializeHook( "", "Effect/client", "Run", Hooks::Run );

}