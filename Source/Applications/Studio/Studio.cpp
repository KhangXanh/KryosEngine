/**
 * @file Application/Studio/Studio.cpp
 * This is a Kryos's Application file
 */

#define USE_CORE_LOG
#include "Engine/Kryos.hpp"

namespace Kryos
{
    class KryosStudio : public Application
    {
    public:
        KryosStudio() : Application("KryoStudio")
        {
            WindowProperties launcherProps{"Kryos Launcher", 1280, 720};
            mLauncherID = GetWindowManager()->AddWindow(launcherProps);

            WindowProperties editorProps{"Editor", 1280, 720};
            mEditorID = GetWindowManager()->AddWindow(editorProps);
        }

        void CustomOnUpdate(Float32 dt) override
        {
            if (GetInputSystem()->IsKeyPressed(Key::A))
            {
                KS_CORE_TRACE("A is pressing");
            }
            if (GetInputSystem()->IsKeyPressed(Key::B))
            {
                KS_CORE_TRACE("B is pressing");
            }
            if (GetInputSystem()->IsMousePressed(MouseButton::LeftButton))
            {
                KS_CORE_TRACE("Left button is pressed");
            }
        }

        void CustomShutdown() override
        {

        }
    
    private:
        WindowID mLauncherID;
        WindowID mEditorID;
    };

    Application* CreateApplication()
    {
        return new KryosStudio();
    }
}