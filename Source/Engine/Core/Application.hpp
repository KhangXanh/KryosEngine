/**
 * @file Engine/Core/Application.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Window.hpp"
#include "LayerStack.hpp"

namespace Kryos
{
    /**
     * @brief
     * A class for managing the engine system (Base class)
     * @note
     * ```text
     * - We will create an application which inherit from 'Application' outside
     * ```
     */
    class KRYOS_API Application
    {
    public:
        Application();
        virtual ~Application() = default;

        /**
         * @brief
         * This function runs the application while it should running (mRunning == true)
         */
        void Run();

    protected:
        /**
         * @brief
         * This function can be modify (override) on each application to create custom update function
         */
        virtual void pCustomOnUpdate(Float32 dt) {};

        /**
         * @brief
         * This function can be modify (override) on each application to create custom shutdown function
         */
        virtual void pCustomShutdown() {};

        virtual void pCustomOnEvent(IEvent& event) {}

        LayerStack *GetLayerStack() { return mLayerStack.get(); }
        WindowManager *GetWindowManager() { return mWindowManager.get(); }

    private:
        /**
         * @brief
         * This function updates the application in run's loop
         * @param[in] dt Delta time (a.k.a the time between each frame)
         * @note
         * ```text
         * - It calls the 'CustomOnUpdate(dt)' and do its stuff
         * ```
         */
        void hInternalOnUpdate(Float32 dt);
        /**
         * @brief
         * This function shutdowns the application
         * @note
         * ```text
         * - It calls the 'CustomShutdown()' and do its stuff
         * ```
         */
        void hInternalShutdown();

    private:
        bool mRunning;

        TUniquePtr<WindowManager> mWindowManager;
        TUniquePtr<LayerStack> mLayerStack;
    };

    /**
     * @brief
     * This function creates a new applcation
     * @note
     * ```text
     * - Client (other applications) will define it outside of the engine
     * - Meaning this function is a bridge from engine to the application
     * ```
     */
    Application *CreateApplication();
}

int main(int argc, char **argv);