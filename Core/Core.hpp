#pragma once


#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <memory.h>
#include <map>
#include <mutex>
#include <vector>
#include <atomic>
#include <array>

#include <Utils/Memory.hpp>
#include <Utils/Input.hpp>
#include <Utils/Crt/LazyImporter.hpp>
#include <Utils/Xorstr/Xorstr.hpp>
#include <Utils/Crt/Crt.hpp>
#include <Utils/Pattern.hpp>
#include <Il2Cpp/Il2Cpp.hpp>

#include <Core/Sdk/RustSDK.hpp>

#include <Rendering/Rendering.hpp>

#include <Settings/Settings.hpp>
#include <Core/Features/Features.hpp>
#include <Menu/Menu.hpp>

#include <Core/Present/Present.hpp>
#include <Core/Hooks/Hooks.hpp>



class Core
{
public:
	struct Tracer
	{
		Vector3 start;
		Vector3 end;
		float thickness;
	};


	struct Box
	{
		Vector2 pos;
		Vector2 size;
	};

	struct Bounds
	{
		float left, right, top, bottom;
	};

	std::mutex tracer_mutex;
	std::mutex entity_mutex;
	std::vector< Tracer > m_tracer;
	std::vector< uintptr_t > m_entities;

	bool cache_finished = false;

	ListDictionary* clientEntities = nullptr;
	Vector3 manipulation_angle = { 0 , 0 , 0 };
	AssetBundle* Bundle = nullptr;
	BasePlayer* Local = nullptr;
	BasePlayer* Target = nullptr;
	BaseProjectile* heldEntity = nullptr;
	inline static Bounds bounds = { 0, 0, 0 ,0 };

	Shader* PlayerShader = nullptr;
	Shader* HandShader = nullptr;


	auto GetBounds( BasePlayer* player ) -> Bounds;
	auto CacheBounds( BasePlayer* player ) -> void;
	auto Cache( ) -> void;
	auto OnFrame( ) -> void;
	auto World2Screen( Vector3 world ) -> Vector3;
	auto CalculateFov( Vector3 position ) -> float;
	auto LineCast( Vector3 a, Vector3 b ) -> bool;

	inline static Core* m_core;

} inline g_ctx;