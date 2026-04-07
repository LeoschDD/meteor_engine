#include "platform/linux/LinuxWindow.hpp"

void meteor::LinuxWindow::Init()
{
    if (!glfw_initialized)
    {
        int success = glfwInit();
        METEOR_CORE_ASSERT(success, "Failed to initialize glfw");
        glfw_initialized = true;
    }
    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    METEOR_CORE_INFO("Window created: {0} ({1}, {2})", title_, width_, height_);

    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, this);
    SetVSync(true);
}

void meteor::LinuxWindow::Shutdown()
{
    glfwDestroyWindow(window_);
    METEOR_CORE_INFO("Window closed");
}

meteor::LinuxWindow::LinuxWindow(uint32_t width, uint32_t height, const std::string& title)
    : width_(width)
    , height_(height)
    , title_(title)
{
    Init();
}

meteor::LinuxWindow::~LinuxWindow()
{
    Shutdown();
}

void meteor::LinuxWindow::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(window_);
}

void meteor::LinuxWindow::SetVSync(bool enabled)
{
    if(enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
    vsync_ = enabled;
}

void meteor::LinuxWindow::SetEventCallback(const std::function<void(Event &)> &callback)
{
}