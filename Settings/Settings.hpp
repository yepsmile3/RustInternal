#pragma once
#include <Core/Core.hpp>

#define SETTING( x ) bool x = false;


class Settings
{
public:
	SETTING( AimbotEnabled );
	SETTING( PSilent );
	SETTING( Prediction );
	SETTING( Manipulation );
	SETTING( AutoFire );
	SETTING( FastBullet );
	SETTING( NoRecoil );
	SETTING( InstaKill );
	SETTING( BulletTp );
	SETTING( BoneOverride );

	SETTING( UseBounds );
	SETTING( NameTag );
	SETTING( CornerBox );
	SETTING( Box );
	SETTING( HealthBarSide );
	SETTING( HealthBarBottom );
	SETTING( Chams );
	SETTING( Skeleton );
	SETTING( WeaponInfo );
	SETTING( LocalPlayerCham );

	SETTING( WeaponChams );
	SETTING( BulletTracers );

	SETTING( Zoom );
	SETTING( WaterMark );
	SETTING( CrossHair );
	SETTING( CustomHitEffect );
	SETTING( HitMarker );
	SETTING( HitSounds );
	SETTING( NoShootAnim );
	SETTING( NoBobbing );
	SETTING( DamageIndicator );
	SETTING( OmniSprint );
	SETTING( SkinChanger );
	SETTING( BunnyHop );
	SETTING( SpeedHack );
	SETTING( HighJump );
	SETTING( AntiAim );
	SETTING( InterActiveDebug );
	SETTING( LootCourpseTW )
	SETTING( AdminAccess );
	SETTING( AlwaysDay );
	SETTING( FullBright );
	SETTING( BrightStars );
	SETTING( Reach );
	SETTING( InfiniteJump );
	SETTING( NoAttackRestriction );
	SETTING( AntiFlyhack );
	SETTING( FastLoot );
	SETTING( InstaBow );
	SETTING( FovToggle );
	SETTING( Reflect );

	float fov = 180.f;
	inline static float AimbotFov = 120.f;
	float cacheTime = 0.f;

	SETTING( Unload );
} inline settings;