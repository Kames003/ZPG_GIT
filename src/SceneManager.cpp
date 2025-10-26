#include "SceneManager.h"
#include "Scene.h"
#include "Light.h"

SceneManager::SceneManager() : activeSceneIndex(0)
{
    printf("SceneManager created\n");
}

void SceneManager::addScene(Scene* scene)
{
    scenes.push_back(scene);
    printf("Scene %d added to SceneManager\n", (int)scenes.size());
}

void SceneManager::switchToScene(int index)
{
    if (index >= 0 && index < (int)scenes.size())
    {
        activeSceneIndex = index;

        // ✅ PRIDANÉ: Notifikuj všetky svetlá v novej scéne!
        Scene* scene = scenes[index];
        if (scene) {
            std::vector<Light*>& lights = scene->getLights();
            for (Light* light : lights) {
                if (light) {
                    light->notifyAll();  // ← Observer pattern!
                }
            }
        }

        printf("\n>>> SWITCHED TO SCENE %d <<<\n", index + 1);
        printf("    Notified %d lights\n\n", scene ? scene->getLightCount() : 0);
    }
    else
    {
        printf("ERROR: Scene index %d out of range (0-%d)\n", 
               index, (int)scenes.size() - 1);
    }
}

void SceneManager::switchToNextScene()
{
    if (scenes.empty()) return;
    
    int nextIndex = (activeSceneIndex + 1) % scenes.size();
    switchToScene(nextIndex);
}

void SceneManager::switchToPreviousScene()
{
    if (scenes.empty()) return;
    
    int prevIndex = (activeSceneIndex - 1 + scenes.size()) % scenes.size();
    switchToScene(prevIndex);
}

Scene* SceneManager::getActiveScene()
{
    if (activeSceneIndex >= 0 && activeSceneIndex < (int)scenes.size())
    {
        return scenes[activeSceneIndex];
    }
    return nullptr;
}

void SceneManager::drawActiveScene()
{
    Scene* active = getActiveScene();
    if (active)
    {
        active->draw();
    }
}

void SceneManager::clear()
{
    scenes.clear();
    activeSceneIndex = 0;
}