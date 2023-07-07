#pragma once

#include "renderer/heim_font.h"

typedef struct HeimUiText {
    HeimFont* font;
    char* text;
} HeimUiText;