#include <Sandbox.hpp>

class EditorApp : public meteor::Application
{
public:
    void OnRender() override
    {
        meteor::Application::OnRender();


    }

private:

};

int main()
{
    EditorApp app;
    app.Run();
}