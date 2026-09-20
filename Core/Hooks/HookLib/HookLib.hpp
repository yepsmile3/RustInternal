#pragma once
#include <Core/Core.hpp>
#include <Utils/Minhook/include/minhook.h>

class Pointer
{
private:
	uintptr_t m_original { 0 };
public:
	auto InitialiseHook( void* ourFunction, const char* nameSpace, const char* klass, const char* method, int argCount = -1 ) -> void
	{
		auto il2cppMethod = *reinterpret_cast< uintptr_t* >( Il2CppWrapper::GetClassFromName( nameSpace, klass )->GetMethodFromName( method, argCount ) );
		m_original = *reinterpret_cast< uintptr_t* >( il2cppMethod );
		*reinterpret_cast< void** >( il2cppMethod ) = ourFunction;
	}
};

class VMT
{
private:
	uintptr_t m_original { 0 };
public:
	auto InitialiseHook( void* ourFunction, const char* nameSpace, const char* klass, const char* method, int argCount = -1 ) -> void
	{
		auto searchMethod = *reinterpret_cast< uintptr_t* >( Il2CppWrapper::GetClassFromName( nameSpace, klass )->GetMethodFromName( method, argCount ) );
		if ( !method )
			return;
		const auto classBase = *reinterpret_cast< uintptr_t* >( Il2CppWrapper::GetClassFromName( nameSpace, klass ) );

		if ( method == ourFunction )
			return;

		for ( auto idx { classBase }; idx <= classBase + 0x1500; idx += 0x1 ) {
			const auto Address = *reinterpret_cast< uintptr_t* >( idx );
			if ( Address == searchMethod )
			{
				m_original = searchMethod;
				*reinterpret_cast< uintptr_t* >( idx ) = uintptr_t( ourFunction );
			}
		}
	}


	template< typename t>
	auto get_original( ) -> t
	{
		return reinterpret_cast< t >( m_original );
	}

};

class HookLib
{
private:
	uintptr_t m_original { 0 };
public:

	static auto Awake( ) -> bool
	{
		if ( MH_Initialize( ) != MH_OK )
			return false;

		return true;
	}

	auto InitializeHook( const char* name_space, const char* klass, const char* method, void* our_function ) -> void
	{
		auto function = *reinterpret_cast< void** >( Il2CppWrapper::GetClassFromName( name_space, klass )->GetMethodFromName( method ) );
		if ( !function )
			return;

		MH_CreateHook( ( void* )function, our_function, ( void** ) &m_original );
		MH_EnableHook( function );
	}


	template< typename t>
	auto get_original( ) -> t
	{
		return reinterpret_cast< t >( m_original );
	}
};