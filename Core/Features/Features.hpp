#pragma once
#include <Core/Core.hpp>



class Features
{
public:

	static auto Object( ) -> Features*
	{
		Features obj;
		return &obj;
	}

	auto RenderPlayer( BasePlayer* player ) -> void;
	auto SunReflect( ) -> Vector3;
	auto Prediction( ) -> Vector3;
	auto BulletTp( Projectile* projectile ) -> void;
	auto BulletTracers( ) -> void;
	auto InstantKill( ) -> void;
	auto FindManipulationAngle( ) -> void;
};