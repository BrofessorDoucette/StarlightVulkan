#pragma once

#include "core/logger.hpp"
#include "rendering/window.hpp"
#include "rendering/context.hpp"

namespace Rendering {


    class Surface {

    public:

        Surface(Window& t_window, Context& t_context);
        ~Surface();

        Surface(const Surface& surface) = delete;
        Surface& operator=(const Surface& surface) = delete;

        void create_surface();

        vk::SurfaceKHR surface() const; //Use only for temporary access to the vulkan surface within other rendering classes.

    private:

        //Dependencies
        Window& m_window;
        Context& m_context;

        //Members
        vk::SurfaceKHR m_surface {nullptr};



    };


}
