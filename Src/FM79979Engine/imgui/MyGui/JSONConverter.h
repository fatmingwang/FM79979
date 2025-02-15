#pragma once

#include "../imgui.h"


namespace nlohmann
{
    template <>
    struct adl_serializer<Vector3>
    {
        static void to_json(json& j, const Vector3& v)
        {
            j = json{ {"x", v.x}, {"y", v.y},{"z", v.z} };
        }

        static void from_json(const json& j, Vector3& v)
        {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
        }
    };
    template <>
    struct adl_serializer<ImVec2>
    {
        static void to_json(json& j, const ImVec2& v)
        {
            j = json{ {"x", v.x}, {"y", v.y} };
        }

        static void from_json(const json& j, ImVec2& v)
        {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
        }
    };
    template <>
    struct adl_serializer<ImVec4>
    {
        static void to_json(json& j, const ImVec4& v)
        {
            j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z}, {"w", v.w} };
        }

        static void from_json(const json& j, ImVec4& v)
        {
            j.at("x").get_to(v.x);
            j.at("y").get_to(v.y);
            j.at("z").get_to(v.z);
            j.at("w").get_to(v.w);
        }
    };
}


namespace nlohmann
{
    template <>
    struct adl_serializer<ImGuiStyle>
    {
        // Serialize ImGuiStyle to JSON
        static void to_json(json& j, const ImGuiStyle& style)
        {
            j = json{
                {"Alpha", style.Alpha},
                {"DisabledAlpha", style.DisabledAlpha},
                {"WindowPadding", {style.WindowPadding.x, style.WindowPadding.y}},
                {"WindowRounding", style.WindowRounding},
                {"WindowBorderSize", style.WindowBorderSize},
                {"WindowMinSize", {style.WindowMinSize.x, style.WindowMinSize.y}},
                {"WindowTitleAlign", {style.WindowTitleAlign.x, style.WindowTitleAlign.y}},
                {"ChildRounding", style.ChildRounding},
                {"ChildBorderSize", style.ChildBorderSize},
                {"PopupRounding", style.PopupRounding},
                {"PopupBorderSize", style.PopupBorderSize},
                {"FramePadding", {style.FramePadding.x, style.FramePadding.y}},
                {"FrameRounding", style.FrameRounding},
                {"FrameBorderSize", style.FrameBorderSize},
                {"ItemSpacing", {style.ItemSpacing.x, style.ItemSpacing.y}},
                {"ItemInnerSpacing", {style.ItemInnerSpacing.x, style.ItemInnerSpacing.y}},
                {"IndentSpacing", style.IndentSpacing},
                {"ColumnsMinSpacing", style.ColumnsMinSpacing},
                {"ScrollbarSize", style.ScrollbarSize},
                {"ScrollbarRounding", style.ScrollbarRounding},
                {"GrabMinSize", style.GrabMinSize},
                {"GrabRounding", style.GrabRounding},
                {"TabRounding", style.TabRounding},
                {"TabBorderSize", style.TabBorderSize},
                {"TabMinWidthForCloseButton", style.TabMinWidthForCloseButton},
                {"DisplayWindowPadding", {style.DisplayWindowPadding.x, style.DisplayWindowPadding.y}},
                {"DisplaySafeAreaPadding", {style.DisplaySafeAreaPadding.x, style.DisplaySafeAreaPadding.y}},
                {"MouseCursorScale", style.MouseCursorScale},
                {"AntiAliasedLines", style.AntiAliasedLines},
                {"AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex},
                {"AntiAliasedFill", style.AntiAliasedFill},
                {"CurveTessellationTol", style.CurveTessellationTol},
                {"CircleTessellationMaxError", style.CircleTessellationMaxError},
                {"Colors", json::array()}
            };

            for (int i = 0; i < ImGuiCol_COUNT; ++i)
            {
                j["Colors"].push_back({ style.Colors[i].x, style.Colors[i].y, style.Colors[i].z, style.Colors[i].w });
            }
        }

        // Deserialize JSON to ImGuiStyle
        static void from_json(const json& j, ImGuiStyle& style)
        {
            j.at("Alpha").get_to(style.Alpha);
            j.at("DisabledAlpha").get_to(style.DisabledAlpha);

            auto wp = j.at("WindowPadding").get<std::array<float, 2>>();
            style.WindowPadding = ImVec2(wp[0], wp[1]);

            j.at("WindowRounding").get_to(style.WindowRounding);
            j.at("WindowBorderSize").get_to(style.WindowBorderSize);

            auto wms = j.at("WindowMinSize").get<std::array<float, 2>>();
            style.WindowMinSize = ImVec2(wms[0], wms[1]);

            auto wta = j.at("WindowTitleAlign").get<std::array<float, 2>>();
            style.WindowTitleAlign = ImVec2(wta[0], wta[1]);

            j.at("ChildRounding").get_to(style.ChildRounding);
            j.at("ChildBorderSize").get_to(style.ChildBorderSize);
            j.at("PopupRounding").get_to(style.PopupRounding);
            j.at("PopupBorderSize").get_to(style.PopupBorderSize);

            auto fp = j.at("FramePadding").get<std::array<float, 2>>();
            style.FramePadding = ImVec2(fp[0], fp[1]);

            j.at("FrameRounding").get_to(style.FrameRounding);
            j.at("FrameBorderSize").get_to(style.FrameBorderSize);

            auto is = j.at("ItemSpacing").get<std::array<float, 2>>();
            style.ItemSpacing = ImVec2(is[0], is[1]);

            auto iis = j.at("ItemInnerSpacing").get<std::array<float, 2>>();
            style.ItemInnerSpacing = ImVec2(iis[0], iis[1]);

            j.at("IndentSpacing").get_to(style.IndentSpacing);
            j.at("ColumnsMinSpacing").get_to(style.ColumnsMinSpacing);
            j.at("ScrollbarSize").get_to(style.ScrollbarSize);
            j.at("ScrollbarRounding").get_to(style.ScrollbarRounding);
            j.at("GrabMinSize").get_to(style.GrabMinSize);
            j.at("GrabRounding").get_to(style.GrabRounding);
            j.at("TabRounding").get_to(style.TabRounding);
            j.at("TabBorderSize").get_to(style.TabBorderSize);
            j.at("TabMinWidthForCloseButton").get_to(style.TabMinWidthForCloseButton);

            auto dwp = j.at("DisplayWindowPadding").get<std::array<float, 2>>();
            style.DisplayWindowPadding = ImVec2(dwp[0], dwp[1]);

            auto dsap = j.at("DisplaySafeAreaPadding").get<std::array<float, 2>>();
            style.DisplaySafeAreaPadding = ImVec2(dsap[0], dsap[1]);

            j.at("MouseCursorScale").get_to(style.MouseCursorScale);
            j.at("AntiAliasedLines").get_to(style.AntiAliasedLines);
            j.at("AntiAliasedLinesUseTex").get_to(style.AntiAliasedLinesUseTex);
            j.at("AntiAliasedFill").get_to(style.AntiAliasedFill);
            j.at("CurveTessellationTol").get_to(style.CurveTessellationTol);
            j.at("CircleTessellationMaxError").get_to(style.CircleTessellationMaxError);

            const auto& colors = j.at("Colors");
            for (int i = 0; i < ImGuiCol_COUNT; ++i)
            {
                const auto& color = colors.at(i).get<std::array<float, 4>>();
                style.Colors[i] = ImVec4(color[0], color[1], color[2], color[3]);
            }
        }
    };

}