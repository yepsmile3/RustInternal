#include "Present.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM param, LPARAM l_param );

auto __stdcall Present::WndProc( const HWND hwnd, unsigned int msg, uintptr_t param, long long l_param ) -> long long
{
	if ( true && ImGui_ImplWin32_WndProcHandler( hwnd, msg, param, l_param ) )
		return true;

	return CallWindowProcA( o_wnd_proc, hwnd, msg, param, l_param );
}

bool present_init { false };
auto Present::PresentHk( IDXGISwapChain* swap_chain, unsigned int sync, unsigned int flags ) -> uintptr_t
{
	if ( !present_init )
	{
		if ( SUCCEEDED( swap_chain->GetDevice( __uuidof( ID3D11Device ), ( void** ) &device ) ) )
		{
			device->GetImmediateContext( &context );
			DXGI_SWAP_CHAIN_DESC sd;
			swap_chain->GetDesc( &sd );
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &pBackBuffer );
			device->CreateRenderTargetView( pBackBuffer, NULL, &main_render_target_view );
			pBackBuffer->Release( );

			o_wnd_proc = ( WNDPROC ) SetWindowLongPtrA( window, GWLP_WNDPROC, ( LONG_PTR ) WndProc );
			ImGui::CreateContext( );
			ImGuiIO& io = ImGui::GetIO( );
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;		
			Rendering::GetInstance( )->m_font = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Tahoma.ttf", 10 );
			ImGui_ImplWin32_Init( window );
			ImGui_ImplDX11_Init( device, context );
		}
		else
			return reinterpret_cast< decltype ( &PresentHk ) > ( o_present ) ( swap_chain, sync, flags );

		present_init = true;
	}

	auto begin_scene = [ & ] ( ) {
		ImGui_ImplDX11_NewFrame( );
		ImGui_ImplWin32_NewFrame( );
		ImGui::NewFrame( );
	};

	auto end_scene = [ & ] ( ) {
		ImGui::Render( );
	};

	auto menu = new Menu;
	if ( GetAsyncKeyState( VK_INSERT ) & 1 )
		menu->menu_show = !menu->menu_show;

	
	begin_scene( );
	{
		menu->Draw( );
	
		if ( Core::m_core->Local != nullptr )
		{
			Core::m_core->OnFrame( );
		}

	}
	end_scene( ); 
	
	context->OMSetRenderTargets( 1, &main_render_target_view, nullptr );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
	return reinterpret_cast< decltype ( &PresentHk ) > ( o_present ) ( swap_chain, sync, flags );
}