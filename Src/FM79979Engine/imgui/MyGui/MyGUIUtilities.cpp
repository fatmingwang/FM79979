#include "MyGUIUtilities.h"
#include "../../Core/AllCoreInclude.h"
#include "../ThirtyParty/ImGuiBuilder/additional.h"
#include <iostream>
#include "../ImGuiRender.h"
#include <fstream>
#include <filesystem>
#include "../../../include/json.hpp"

// Helper function to get the current date
void GetCurrentDate(int& year, int& month, int& day)
{
    time_t t = time(nullptr);
    tm* localTime = localtime(&t);
    year = localTime->tm_year + 1900;
    month = localTime->tm_mon + 1;
    day = localTime->tm_mday;
}


bool CustomInputInt(const char* label, int* value, int step = 1, int step_fast = 1)
{
    bool valueChanged = false;
    ImGui::PushID(label);
    // Create a horizontal group for buttons and the label
    ImGui::BeginGroup();

    // Decrement button
    if (ImGui::Button("-"))
    {
        *value -= step;
        valueChanged = true;
    }
    ImGui::SameLine(0, 2); // Add spacing between buttons and the text

    // Display the value as text
    ImGui::Text("%d", *value);
    ImGui::SameLine(0, 2); // Add spacing between the text and increment button

    // Increment button
    if (ImGui::Button("+"))
    {
        *value += step;
        valueChanged = true;
    }

    // End the group and add the label
    ImGui::SameLine();
    ImGui::TextUnformatted(label);
    ImGui::EndGroup();

    ImGui::PopID(); // Pop the unique ID
    return valueChanged;
}

bool DatePicker(const char* label, int& year, int& month, int& day)
{
    bool valueChanged = false;

    ImGui::Text("%s", label);
    ImGui::SameLine();

    // Month combo box
    //static const char* months[] = {
    //    "January", "February", "March", "April", "May", "June",
    //    "July", "August", "September", "October", "November", "December"
    //};
    static const char* months[] = {
        "1", "2", "3", "4", "5", "6",
        "7", "8", "9", "10", "11", "12"
    };
    const char* currentMonth = months[month - 1];
    if (ImGui::BeginCombo("month", currentMonth))
    {
        for (int i = 0; i < 12; i++)
        {
            bool isSelected = (month - 1 == i);
            if (ImGui::Selectable(months[i], isSelected))
            {
                month = i + 1;
                valueChanged = true;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();


    // Day input
    //valueChanged |= ImGui::InputInt("##day", &day, 1, 1);
    valueChanged |= CustomInputInt("day", &day, 1, 1);

    if (day < 1) day = 31;
    if (day > 31) day = 1; // Simplified validation

    ImGui::SameLine();


    // Year input
    valueChanged |= CustomInputInt("year", &year, 1, 1);
    //valueChanged |= ImGui::InputInt("##year", &year, 1, 10);

    if (year < 1900) year = 1900;
    if (year > 2100) year = 2100;

    return valueChanged;
}


#define	sTest_DATA a, b, m_strName
// Base structure
struct sTest
{
    int a = 0;
    double b = 0.0;
    std::string m_strName;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(sTest, a, b, m_strName)
};

// Derived structure
struct sTest2 : public sTest
{
    int a2 = 0;
    double b2 = 0.0;
    std::string m_strName2;

    // Combine base and derived members for JSON serialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(sTest2, sTest_DATA, a2, b2, m_strName2)
};


void  JsonTesting()
{
    // Deserialize JSON to derived structure
    nlohmann::json newJson = R"({
        "a": 10,
        "b": 1.618,
        "m_strName": "Base Test",
		"m_strName3": "Base Testqoo",
        "a2": 20,
        "b2": 3.14159,
        "m_strName2": "Derived Test"
    })"_json;

    sTest2 newStruct;
    newStruct = newJson.get<sTest2>();
}


bool CheckMouseAndCurrentWindowCollision()
{
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    bool isMouseInside =
        mousePos.x >= windowPos.x &&
        mousePos.x <= (windowPos.x + windowSize.x) &&
        mousePos.y >= windowPos.y &&
        mousePos.y <= (windowPos.y + windowSize.y);

    return isMouseInside;
}

void NumericUpDown(const char* label, int* value, int minValue, int maxValue, int step)
{

    ImGui::PushID(label);

    ImGui::PushItemWidth(250);
    // Create the input field
   // Create the input field
    ImGui::InputInt("##input", value, step, 100, ImGuiInputTextFlags_CharsDecimal);
    bool l_bChenged = false;
    // Detect keyboard input when the input is focused
    if (ImGui::IsItemFocused())
    {
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
        {
            *value += step;  // Increment value when Up key is pressed
            l_bChenged = true;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
        {
            *value -= step;  // Decrement value when Down key is pressed
            l_bChenged = true;
        }
        //if (l_bChenged)
        {
            // Force refresh of the input text display
            ImGui::SetItemAllowOverlap();  // Allow overlap of input field if value changed
        }
    }
    // Clamp the value to the defined range
    if (*value < minValue)
    {
        *value = minValue;
    }
    if (*value > maxValue)
    {
        *value = maxValue;
    }

    ImGui::PopItemWidth();
    ImGui::PopID();
}


void RenderHintLabel(const char* e_strContent)
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.92f, 0.92f, 0.92f, 1.00f));
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(e_strContent);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    ImGui::PopStyleColor(1);
}