/**
 * @file Studio/Layers/Editor/EditorLayer.cpp
 * This is a Kryos Application Implementation file
 */

#include "EditorLayer.hpp"

namespace Kryos
{
    EditorLayer::EditorLayer(WindowManager *winMan)
    {
        mWinMan = winMan;
    }

    void EditorLayer::OnAttach()
    {
        mWindowID = mWinMan->AddWindow(WindowProperties{"Kryos Editor", 1280, 720});
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Float32 dt)
    {

    }

    void EditorLayer::OnEvent(IEvent& event)
    {

    }
}