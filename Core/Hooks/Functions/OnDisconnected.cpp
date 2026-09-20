#include <Core/Hooks/Hooks.hpp>

auto Hooks::OnDisconnected( Connection* _This ) -> void
{
	Hooks::OnDisconnectedhk.get_original< decltype( &OnDisconnected )>( )( _This );

	Core::m_core->clientEntities = nullptr;
	Core::m_core->Bundle = nullptr;
	Core::m_core->heldEntity = nullptr;
	Core::m_core->PlayerShader = nullptr;
	Core::m_core->HandShader = nullptr;
}