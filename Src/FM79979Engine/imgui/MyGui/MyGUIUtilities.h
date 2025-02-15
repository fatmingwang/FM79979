#pragma once
#include <chrono>
void GetCurrentDate(int& year, int& month, int& day);
bool DatePicker(const char* label, int& year, int& month, int& day);
bool CheckMouseAndCurrentWindowCollision();
void NumericUpDown(const char* label, int* value, int minValue = 0, int maxValue = 100, int step = 1);
void RenderHintLabel(const char* e_strContent,const char*e_strLabel = "(?)");

struct sToastMessage
{
    bool show = false;
    std::chrono::steady_clock::time_point startTime;
    float duration = 2.0f; // Duration in seconds
    std::string message;

    void ShowToast(const std::string& msg, float durationSeconds = 2.0f);
    void Render();
};