#pragma once
#include <ImGui/imgui.h>
#include <Poppins.hpp>
#include <wtypes.h>

namespace ImGui_Loader
{	
	static ImFont* Font_Size_17 = nullptr;
	static ImFont* Icon_Font = nullptr;

	inline bool LoadFont() {		
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		io.LogFilename = nullptr;
		io.IniFilename = nullptr;

		// loading fonts;
		mainfont = io.Fonts->AddFontFromMemoryTTF(&mainfonthxd, sizeof mainfonthxd, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
		childtitle = io.Fonts->AddFontFromMemoryTTF(&boldfonthxd, sizeof boldfonthxd, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
		notiffont = io.Fonts->AddFontFromMemoryTTF(&boldfonthxd, sizeof boldfonthxd, 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
		titlefont = io.Fonts->AddFontFromMemoryTTF(&boldfonthxd, sizeof boldfonthxd, 24, NULL, io.Fonts->GetGlyphRangesCyrillic());
		icons = io.Fonts->AddFontFromMemoryTTF(&iconshxd, sizeof iconshxd, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
		arrow = io.Fonts->AddFontFromMemoryTTF(&arrowhxd, sizeof arrowhxd, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());
		logo = io.Fonts->AddFontFromMemoryTTF(&logohxd, sizeof logohxd, 80, NULL, io.Fonts->GetGlyphRangesCyrillic());
		brandfont = io.Fonts->AddFontFromMemoryTTF(&brandfonthxd, sizeof brandfonthxd, 20, NULL, io.Fonts->GetGlyphRangesCyrillic());

		return true;
	}
}

