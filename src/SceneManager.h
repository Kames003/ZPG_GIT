#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include <stdio.h>

class Scene;  // Forward deklarácia

class SceneManager
{
private:
    std::vector<Scene*> scenes;
    int activeSceneIndex;

public:
    SceneManager();
    
    void addScene(Scene* scene);
    void switchToScene(int index);
    void switchToNextScene();
    void switchToPreviousScene();

    Scene* getActiveScene();
    int getActiveSceneIndex() const { return activeSceneIndex; }
    int getSceneCount() const { return scenes.size(); }

    // ← NOVÁ METÓDA!
    Scene* getScene(int index) const {
        if (index >= 0 && index < (int)scenes.size()) {
            return scenes[index];
        }
        return nullptr;
    }

    void drawActiveScene();
    void clear();
};

#endif