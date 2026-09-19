#include "Menu.hpp"

static int selected_tab;
auto Menu::Draw( ) -> void
{
	auto CreateTab = [ & ] ( const char* name, int index, int& selected_tab, ImVec2 size ) {
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.65f, 0.65f, 0.65f, 0.9f ) );
		if ( selected_tab == index )
			ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.f, 1.f, 1.f, 0.95f ) );
		if ( ImGui::Button( name, size ) ) {
			if ( selected_tab == index )
				ImGui::PopStyleColor( );
			ImGui::PopStyleColor( );
			selected_tab = index;
			return;
		}
		if ( selected_tab == index )
			ImGui::PopStyleColor( );
		ImGui::PopStyleColor( );
		ImGui::SameLine( );
	};

	auto Aimbot = [ & ] ( )
	{
		ImGui::Spacing( );

		ImGui::Text( "Aimbot Options" );

		ImGui::BeginChild( "##AimbotOptions", ImVec2( ImGui::GetContentRegionAvail( ).x, 0 ), true, ImGuiWindowFlags_AlwaysUseWindowPadding );
		{
			ImGui::Checkbox( "Enabled", &settings.AimbotEnabled );
			ImGui::Checkbox( "PSilent", &settings.PSilent );
			ImGui::SliderFloat( "AimbotFov", &settings.AimbotFov, 0.f, 360.f );
			ImGui::Checkbox( "MagicBullet", &settings.Manipulation );
			ImGui::Checkbox( "BulletTP", &settings.BulletTp );
			ImGui::Checkbox( "AutoFire", &settings.AutoFire );
			ImGui::Checkbox( "FastBullet", &settings.FastBullet );
			ImGui::Checkbox( "InstantKill", &settings.InstaKill );
			ImGui::Checkbox( "NoRecoil", &settings.NoRecoil );
			ImGui::Checkbox( "Prediction", &settings.Prediction );
			ImGui::Checkbox( "BoneOverride", &settings.BoneOverride );
		}
		ImGui::EndChild( );
	};

	auto ESP = [ & ] ( )
	{
		ImGui::Spacing( );
		ImGui::BeginChild( "##PlayerVisualsChild", ImVec2( 300, 0 ), true, ImGuiWindowFlags_AlwaysUseWindowPadding );
		{
			ImGui::Text( "Player Visuals" );
			ImGui::Checkbox( "UseBounds", &settings.UseBounds );
			ImGui::Checkbox( "ShowName", &settings.NameTag );
			ImGui::Checkbox( "ShowBox", &settings.Box );
			ImGui::Checkbox( "WeaponInfo", &settings.WeaponInfo );
			ImGui::Checkbox( "Corner Box", &settings.CornerBox );
			ImGui::Checkbox( "HealthBarSide", &settings.HealthBarSide );
			ImGui::Checkbox( "HealthBarBottom", &settings.HealthBarBottom );
			ImGui::Checkbox( "Skeleton", &settings.Skeleton );
			ImGui::Checkbox( "Chams", &settings.Chams );
		}
		ImGui::EndChild( );

		ImGui::SameLine( );

		ImGui::BeginChild( "##LocalPlayerVisualsChild", ImVec2( 300, 0 ), true, ImGuiWindowFlags_AlwaysUseWindowPadding );
		{
			ImGui::Text( "Localplayer Visuals" );
			ImGui::Checkbox( "HandChams", &settings.WeaponChams );
			ImGui::Checkbox( "PlayerChams", &settings.LocalPlayerCham );
		}
		ImGui::EndChild( );
	};

	auto WorldESP = [ & ] ( )
	{

	};

	auto Misc = [ & ] ( )
	{
		ImGui::Spacing( );	
		ImGui::BeginChild( "##Misc", ImVec2( 300, 0 ), true, ImGuiWindowFlags_AlwaysUseWindowPadding );
		{
			ImGui::Text( "Weapon" ); 
			ImGui::Checkbox( "BulletTracers", &settings.BulletTracers );
			ImGui::Checkbox( "NoShootAnim", &settings.NoShootAnim );
			ImGui::Checkbox( "NoBobbing", &settings.NoBobbing );
			ImGui::Checkbox( "NoAttackRestriction", &settings.NoAttackRestriction );
			ImGui::Checkbox( "DamageIndicator", &settings.DamageIndicator );
			ImGui::Checkbox( "CustomHitEffect", &settings.CustomHitEffect );
			ImGui::Checkbox( "HitSounds", &settings.HitSounds );
			ImGui::Checkbox( "HitMarker", &settings.HitMarker );
			ImGui::Checkbox( "Insta Bow", &settings.InstaBow );
			ImGui::Checkbox( "Reach", &settings.Reach );
			ImGui::SliderFloat( "cacheTime", &settings.cacheTime, 0, 5 );
		}
		ImGui::EndChild( );

		ImGui::SameLine( );

		ImGui::BeginChild( "##Misc2", ImVec2( 300, 0 ), true, ImGuiWindowFlags_AlwaysUseWindowPadding );
		{
			ImGui::Text( "Movement" );
			ImGui::Checkbox( "InfiniteJump", &settings.InfiniteJump );
			ImGui::Checkbox( "OmniSprint", &settings.OmniSprint );
			ImGui::Checkbox( "BunnyHop", &settings.BunnyHop );
			ImGui::Checkbox( "HighJump", &settings.HighJump );
			ImGui::Checkbox( "Speedhack", &settings.SpeedHack );
			ImGui::Checkbox( "AntiFlyhackKick", &settings.AntiFlyhack );
			ImGui::Checkbox( "InteractiveDebug", &settings.InterActiveDebug );
			

			ImGui::Text( "Other" );
			ImGui::Checkbox( "AntiAim", &settings.AntiAim );
			ImGui::Checkbox( "LootCourpseTW", &settings.LootCourpseTW );
			ImGui::Checkbox( "AdminAccess", &settings.AdminAccess );
			ImGui::Checkbox( "FastLoot", &settings.FastLoot );
			ImGui::Checkbox( "Fullbright", &settings.FullBright );
			ImGui::Checkbox( "Fov Toggle", &settings.FovToggle );
			ImGui::Checkbox( "Skinchanger", &settings.SkinChanger );
			ImGui::Checkbox( "Zoom ( C )", &settings.Zoom );
			ImGui::SliderFloat( "FovSlider", &settings.fov, 0.f, 180.f );
		}
		ImGui::EndChild( );


	};

	auto MainUI = [ & ] ( ) {
		
		ImGui::Begin( "", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar  | ImGuiWindowFlags_NoSavedSettings );
		ImGui::SetWindowSize( ImVec2( 600, 400 ) );
	 	CreateTab( "Aimbot", 0, selected_tab, ImVec2( 100, 20 ) );
		CreateTab( "ESP", 1, selected_tab, ImVec2( 100, 20 ) );
		CreateTab( "WorldESP", 2, selected_tab, ImVec2( 100, 20 ) );
		CreateTab( "Misc", 3, selected_tab, ImVec2( 100, 20 ) );

		switch ( selected_tab )
		{
		case 0:
			Aimbot( );
			break;
		case 1:
			ESP( );
			break;
		case 2:
			WorldESP( );
			break;
		case 3:
			Misc( );
			break;
		}

		ImGui::End( );
	};
	if( menu_show )
		MainUI( );
}