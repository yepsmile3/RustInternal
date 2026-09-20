#include <Core/Hooks/Hooks.hpp>

bool hook { false };
auto Hooks::ClientInput( BasePlayer* _This, InputState* state ) -> void
{
	if ( !Core::m_core->Bundle )
		Core::m_core->Bundle = AssetBundle::LoadFromFile( L"C:\\base.assets", 0, 0 );

	if ( !Core::m_core->Local )
		Core::m_core->Local = LocalPlayer::Entity( );
	/*
		for ( auto idx { 0 }; idx <= Core::m_core->m_entities.size( ); ++idx )
	{
		if ( idx == 0 )
			continue;

		const auto player = reinterpret_cast< BasePlayer* >( Core::m_core->m_entities[ idx ] );
		if ( !player )
			continue;

		if ( settings.Chams )
		{
			auto playerModel = player->playerModel( );
			if ( !playerModel )
				continue;

			auto multiMesh = player->playerModel( )->_multiMesh( );
			if ( !multiMesh )
				continue;

			auto Renderers = multiMesh->Renderers( );
			if ( !Renderers )
				continue;

			for ( auto idx { 0 }; idx < Renderers->size( ); idx++ )
			{
				auto Renderer = Renderers->value( idx );
				if ( !Renderer )
					continue;

				auto Material = Renderer->material( );
				if ( !Material )
					continue;
			
				if ( !Core::m_core->PlayerShader )
					Core::m_core->PlayerShader = Core::m_core->Bundle->LoadAsset< Shader* >( L"chams.shader", Il2CppWrapper::Type( "UnityEngine", "Shader" ) );

				Material->set_shader( Core::m_core->PlayerShader );
			}
		}

	}

	*/

	if ( settings.WeaponChams )
	{
		auto ActiveModel = BaseViewModel::ActiveModel( );
		if ( !ActiveModel )
			return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

		auto RenderList = ActiveModel->GetComponentsInChildren( Il2CppWrapper::Type( "UnityEngine", "Renderer" ) );
		if ( !RenderList )
			return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

		auto size = *reinterpret_cast< int* >( RenderList + 0x18 );

		for ( int idx { 0 }; idx < size; idx++ )
		{
			auto RenderObject = *reinterpret_cast< Renderer** >( RenderList + 0x20 + ( idx * 0x8 ) );
			if ( !RenderObject )
				return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

			auto Material = RenderObject->material( );
			if ( !Material )
				return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

			if ( !Core::m_core->HandShader )
				Core::m_core->HandShader = Core::m_core->Bundle->LoadAsset< Shader* >( L"Wallhack_Fresnel.shader", Il2CppWrapper::Type( "UnityEngine", "Shader" ) );

			if ( Material->name( )->Contains( L"Hand" ) || Material->name( )->Contains( L"Arm" ) || Material->name( )->Contains( L"Glove" ) )
				Material->set_shader( Core::m_core->HandShader );
		}
	}

	if ( settings.FovToggle )
	{
		static auto staticFields = *reinterpret_cast< uintptr_t* >( ConVar::Graphics::StaticClass( ) + 0xb8 );
		if ( !staticFields )
			return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

		if ( settings.Zoom && Input::GetKey( KeyCode::C ) )
			*reinterpret_cast< float* > ( staticFields + 0x18 ) = 35.f;
		else
			*reinterpret_cast< float* > ( staticFields + 0x18 ) = settings.fov;
	}

	if ( settings.OmniSprint )
	{
		if ( auto modelState = Core::m_core->Local->modelState( ) )
			modelState->SetFlag( ModelStateFlag::Sprinting );	
	}

	if ( settings.Manipulation && input::is_pressed( VK_XBUTTON1 ) )
		Core::m_core->Local->clientTickInterval( ) = 0.99f;
	else
		Core::m_core->Local->clientTickInterval( ) = 0.05f;

	BaseProjectile* heldEntity = nullptr;
	if ( auto item = Core::m_core->Local->item( ) )
	{
		if ( item->info( )->category( ) == ItemCategory::Weapon )
		{
			heldEntity = item->heldEntity( );
			if ( !heldEntity )
				return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

			auto il2cppClass = reinterpret_cast< Il2CppClass* >( heldEntity );

			if ( settings.NoShootAnim )
			{
				if ( auto ActiveModel = BaseViewModel::ActiveModel( ) )
				{
					if ( Core::m_core->Local->input( )->state( )->IsDown( BUTTON::FIRE_PRIMARY ) )
						ActiveModel->animator( )->set_speed( -1 );
				}
			}

			if ( settings.NoBobbing )
			{	
				if ( auto ActiveModel = BaseViewModel::ActiveModel( ) )
				{
					auto bob = ActiveModel->bob( );
					if ( !bob )
						return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

					auto lower = ActiveModel->lower( );
					if ( !lower )
						return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

					auto sway = ActiveModel->sway( );
					if ( !sway )
						return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

					bob->bobAmountRun( ) = 0.f;
					bob->bobAmountWalk( ) = 0.f;
					bob->bobSpeedRun( ) = 0.f;
					bob->bobSpeedWalk( ) = 0.f;
					lower->shouldLower( ) = false;				
				}
			}

			if ( settings.InstaKill && Input::GetKey( KeyCode::Mouse3 ) ) 
				Features::Object( )->InstantKill( );

			if ( settings.Manipulation && input::is_pressed( VK_XBUTTON1 ) ) 
				Features::Object( )->FindManipulationAngle( );
			
			if ( settings.Reach && heldEntity->IsA( BaseMelee::StaticClass( ) ) )
			{
				if( auto baseMelee = reinterpret_cast< BaseMelee* >( heldEntity ) )
				{
					baseMelee->attackRadius( ) = 9.f;
					baseMelee->deployDelay( ) = 0.f;
					baseMelee->isAutomatic( ) = true;
				}
			}

			if ( settings.NoRecoil && !heldEntity->IsA( BaseMelee::StaticClass( ) ) );
			{
				if ( heldEntity )
				{
					auto recoil = heldEntity->recoil( );
					if ( !recoil )
						return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

					auto newRecoil = recoil->newRecoilOverride( );
					if ( !newRecoil )
						return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

					newRecoil->recoilPitchMax( ) = 0.f;
					newRecoil->recoilPitchMin( ) = 0.f;
					newRecoil->recoilYawMax( ) = 0.f;
					newRecoil->recoilYawMin( ) = 0.f;
				}		
			}

			if ( heldEntity != nullptr )
				Core::m_core->heldEntity = heldEntity;
		}
	}

	if ( settings.FullBright )
	{
		if ( auto Instance = TOD_Sky::Instance( ) )
		{
			auto Night = Instance->Night( );
			if ( !Night )
				return Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );

			Night->LightIntensity( ) = 25.f;
			Night->ReflectionMultiplier( ) = 5.f;
		}

	}

	Hooks::ClientInputhk.get_original< decltype( &ClientInput )>( )( _This, state );
}