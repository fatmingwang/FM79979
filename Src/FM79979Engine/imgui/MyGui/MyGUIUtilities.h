#pragma once

void GetCurrentDate(int& year, int& month, int& day);
bool DatePicker(const char* label, int& year, int& month, int& day);
bool CheckMouseAndCurrentWindowCollision();
void NumericUpDown(const char* label, int* value, int minValue = 0, int maxValue = 100, int step = 1);
void RenderHintLabel(const char* e_strContent);