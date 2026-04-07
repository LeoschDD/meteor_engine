#include "Window.hpp"

static bool glfw_initialized = false;

void meteor::Window::Init()
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

    SetEventCallbacks();
}

void meteor::Window::Shutdown()
{
    glfwDestroyWindow(window_);
    METEOR_CORE_INFO("Window closed");
}

void meteor::Window::SetEventCallbacks()
{
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->event_queue_.push_back(std::make_unique<WindowResizeEvent>(width, height));
    });
    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->event_queue_.push_back(std::make_unique<WindowCloseEvent>());
    });
    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) self->event_queue_.push_back(std::make_unique<KeyPressedEvent>(key, false));
        if (action == GLFW_REPEAT) self->event_queue_.push_back(std::make_unique<KeyPressedEvent>(key, true));
        if (action == GLFW_RELEASE) self->event_queue_.push_back(std::make_unique<KeyReleasedEvent>(key));
    });
    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double x, double y)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->event_queue_.push_back(std::make_unique<MouseMovedEvent>(x, y));
    });
    glfwSetScrollCallback(window_, [](GLFWwindow* window, double x, double y)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->event_queue_.push_back(std::make_unique<MouseScrolledEvent>(x, y));
    });
    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) self->event_queue_.push_back(std::make_unique<MouseButtonPressedEvent>(button));
        if (action == GLFW_RELEASE) self->event_queue_.push_back(std::make_unique<MouseButtonReleasedEvent>(button));
    });
}

meteor::Window::Window(int width, int height, const std::string& title)
    : width_(width)
    , height_(height)
    , title_(title)
{
    Init();
}

meteor::Window::~Window()
{
    Shutdown();
}

void meteor::Window::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(window_);
}

void meteor::Window::SetVSync(bool enabled)
{
    enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
    vsync_ = enabled;
}