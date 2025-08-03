#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

inline void LeaveLine(int num = 1)
{
	for (int i = 0; i < num; i++)
	{
		ImGui::Dummy(ImVec2());
	}
}

template <typename type>
inline void ColorEditor(const char* label, type* color, float roundness = 2.0f)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, roundness);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(239, 73, 88, 255).Value);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(229, 63, 78, 255).Value);
    ImGui::ColorEdit4(label, (float*)color);
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(2);
}

//inline bool ComboBox(const char* label, const char* combo_items, int* current_index, const char* const items[], int items_size, float roundness = 2.0f, float width = 150.0f)
//{
//    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(15, 15, 15, 155).Value);
//    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(15, 15, 15, 155).Value);
//    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(15, 15, 15, 155).Value);
//
//    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, roundness);
//
//    ImGui::SetNextItemWidth(width);
//    bool is_open;
//
//    if (is_open)
//    {
//        for (int i = 0; i < items_size; ++i)
//        {
//            bool is_selected = (i == *current_index);
//            if (is_selected)
//            {
//                ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 215, 0).Value);
//            }
//
//            if (ImGui::Selectable(items[i], is_selected))
//            {
//                *current_index = i;
//            }
//
//            if (is_selected)
//            {
//                ImGui::SetItemDefaultFocus();
//            }
//
//            if (is_selected)
//            {
//                ImGui::PopStyleColor();
//            }
//        }
//        ImGui::EndCombo();
//    }
//
//    ImGui::PopStyleVar(1);
//    ImGui::PopStyleColor(3);
//
//    return is_open;
//}

inline bool CustomButton(const char* label, ImVec2 size = ImVec2(150, 40)) // width, height
{
    bool clicked = ImGui::Button(label, size);
    return clicked;
}

inline void CenterText(const char* format, const float y_padding = 0.0f, ImColor color = ImColor(255, 255, 255))
{
    const ImVec2 text_size = ImGui::CalcTextSize(format);
    ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (text_size.x / 2));


    if (y_padding > 0.0f)
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y_padding);
    ImGui::TextColored(color, format);
}

inline void Checkbox(const char* format, bool* value)
{
    ImColor CheckMarkColor = ImColor(239, 73, 88, 255);
    ImColor CheckBoxBGColor = ImColor(31, 30, 31, 255);
    ImColor CheckBoxHoveredBGColor = ImColor(27, 25, 22, 255);
    ImColor CheckBoxActiveBGColor = ImColor(31, 30, 31, 255);

	int colorCount = 3; // Number of colors to push when unchecked

	if (*value)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, CheckBoxBGColor.Value);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, CheckBoxHoveredBGColor.Value);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, CheckBoxActiveBGColor.Value);
		ImGui::PushStyleColor(ImGuiCol_CheckMark, CheckMarkColor.Value);
		colorCount = 4; // Number of colors to push when checked
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg, CheckBoxBGColor.Value);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, CheckBoxHoveredBGColor.Value);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, CheckBoxActiveBGColor.Value);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.5f, 1.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

	ImGui::Checkbox(format, value);

	ImGui::PopStyleColor(colorCount); // Pop the exact number of colors that were pushed
	ImGui::PopStyleVar(2); // Pop the style variables

	// If spacing is needed, specify the size in ImVec2(x, y)
	ImGui::Dummy(ImVec2());
}

inline void SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f")
{
    ImColor SliderBGColor = ImColor(30, 29, 30, 255);
    ImColor SliderGrabColor = ImColor(239, 73, 88, 255);
    ImColor SliderGrabActiveColor = ImColor(219, 63, 78, 255);

    // Push custom style variables and colors
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 150.0f); // Adjust frame rounding
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.1, 0.1)); // Adjust padding
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 50.0f); // Adjust grab rounding

    ImGui::PushStyleColor(ImGuiCol_FrameBg, SliderBGColor.Value); // Background color of the slider track
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, SliderBGColor.Value); // Background color of the slider track when hovered
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, SliderBGColor.Value); // Background color of the slider track when clicked
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, SliderGrabColor.Value);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, SliderGrabActiveColor.Value);

    ImGui::SliderFloat(label, v, v_min, v_max, format);

    // Pop the custom style colors and variables
    ImGui::PopStyleColor(5); // Pop the 5 style colors
    ImGui::PopStyleVar(3);   // Pop the 2 style variables
}

inline void SliderInt(const char* label, int* v, int v_min, float v_max, const char* format = "%d")
{
    ImColor SliderBGColor = ImColor(30, 29, 30, 255);
    ImColor SliderGrabColor = ImColor(239, 73, 88, 255);
    ImColor SliderGrabActiveColor = ImColor(219, 63, 78, 255);

    // Push custom style variables and colors
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 150.0f); // Adjust frame rounding
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.1, 0.1)); // Adjust padding
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 50.0f); // Adjust grab rounding

    ImGui::PushStyleColor(ImGuiCol_FrameBg, SliderBGColor.Value); // Background color of the slider track
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, SliderBGColor.Value); // Background color of the slider track when hovered
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, SliderBGColor.Value); // Background color of the slider track when clicked
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, SliderGrabColor.Value);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, SliderGrabActiveColor.Value);

    ImGui::SliderInt(label, v, v_min, v_max, format);

    // Pop the custom style colors and variables
    ImGui::PopStyleColor(5); // Pop the 5 style colors
    ImGui::PopStyleVar(3);   // Pop the 2 style variables
}

inline void PlainText(const char* label, ImColor color = ImColor(255, 255, 255, 255))
{
    ImGui::TextColored(color, label);
}

inline void SetCustomStyle()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImGuiStyle* style = &ImGui::GetStyle();

    style->ItemSpacing = ImVec2(8, 16);

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
    colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
    colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
    colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
