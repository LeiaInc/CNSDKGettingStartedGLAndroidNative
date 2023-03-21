#pragma once

#include "leia/sdk/api.h"

#define IMGUI_API LEIASDK_API

struct ImGuiContext;
extern thread_local ImGuiContext* g_ImGuiTLS;
#define GImGui g_ImGuiTLS
