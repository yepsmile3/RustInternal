#pragma once
#include <Core/Core.hpp>
#include <Core/Hooks/HookLib/HookLib.hpp>


#define VMT_CREATE( type, name, args ) inline static VMT name##hk; static type name args;
#define CREATE( type, name, args ) inline static HookLib name##hk; static type name args;


class Hooks
{
private:
	CREATE( void, DoMovement, ( Projectile* _This, float deltaTime ) )
	CREATE( void, Run, ( void* _This, String, Vector3, Vector3, Vector3 ) )
	CREATE( void, OnInput, ( BaseProjectile* _This ) )
	CREATE( void, LaunchProjectileClientside, ( BaseProjectile* _This, ItemDefinition* ammo, int projectileCount, float projSpreadaimCone ) )
	CREATE( void, ClientUpdate, ( BasePlayer* _This ) )
	CREATE( void, SendProjectileAttack, ( BasePlayer*, ProtoBuf::PlayerProjectileAttack* attack ) )
	CREATE( void, OnDisconnected, ( Connection* _This ) )
	CREATE( void, HandleJumping, ( PlayerWalkMovement* _This, ModelState* state, bool wantsJump, bool jumpInDirection ) )
	CREATE( void, ProjectileUpdate, ( Projectile* _This ) )
	CREATE( void, PerformanceUI, ( void* _This ) )
	CREATE( void, ClientInput, ( BasePlayer*, InputState* state ) )
	CREATE( void, SendSignalBroadcast, ( void* _This, int Signal, String arg ) )
	CREATE( void, OnAttacked, ( BaseCombatEntity* _This, HitInfo* hitInfo ) )
	CREATE( Vector3, PSilent, ( float aimCone, Vector3 direction, bool anywhereInside ) )
	CREATE( void, Play, ( void* _This, String stateName ) )
	CREATE( void, LaunchProjectile, ( BaseProjectile* _This ) )
	CREATE( void, HandleRunDuckCrawl, ( PlayerWalkMovement* _This, ModelState* state, bool wantsRun, bool wantsDuck, bool wantsCrawl ) )
public:
	static auto Start( ) -> void;
};