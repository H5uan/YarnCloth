#include "GLCore.h"
#include "EditorLayer.h"
#include "Core/Application.h"

using namespace GLCore;

class App final : public Application {
public:
    explicit App() : Application("Dyna-Render") {
        PushLayer(new EditorLayer());
    }

    void Reset() {
    }

    void SwitchScene(unsigned int sceneIndex) {
    }

    void SetScenes(const vector<shared_ptr<Scene> > &scenes);

    glm::ivec2 windowSize();

};

int main(int argc, char **argv) {
    const auto app = std::make_unique<App>();
    app->Run();
}
