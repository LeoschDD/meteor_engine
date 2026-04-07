#include "platform/windows/WindowsWindow.hpp"

void meteor::WindowsWindow::Init()
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

void meteor::WindowsWindow::Shutdown()
{
    glfwDestroyWindow(window_);
    METEOR_CORE_INFO("Window closed");
}

meteor::WindowsWindow::WindowsWindow(uint32_t width, uint32_t height, const std::string& title)
    : width_(width)
    , height_(height)
    , title_(title)
{
    Init();
}

meteor::WindowsWindow::~WindowsWindow()
{
    Shutdown();
}

void meteor::WindowsWindow::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(window_);
}

void meteor::WindowsWindow::SetVSync(bool enabled)
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

void meteor::WindowsWindow::SetEventCallback(const std::function<void(Event &)> &callback)
{
}