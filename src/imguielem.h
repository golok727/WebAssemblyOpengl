#pragma once
#include <functional>
#include "imgui/imgui.h"
namespace ImGui
{

  inline void AppAutoResize(const char *label, bool *pOpen, std::function<void()> drawChildren)
  {
    if (!pOpen)
      return;

    if (!ImGui::Begin(label, pOpen, ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::End();
      return;
    }

    drawChildren();

    ImGui::End();
  }

}