#pragma once

#include <string>
#include <functional>
#include <memory>

#include "Common.hpp"
#include "Core/Actor.h"

class Scene {
public:
    virtual ~Scene() = default;

    std::string name = "BaseScene";

    virtual void PopulateActors() = 0;

    CallbackHandler<void()> onEnter;
    CallbackHandler<void()> onExit;

protected:
    template<class T>
    void ModifyParameter(T *ptr, T value) {
        onEnter.Register([this, ptr, value]() {
            T prev = *ptr;
            *ptr = value;
            onExit.Register([ptr, prev, value]() {
                //fmt::print("Revert ptr[{}] from {} to value {}\n", (int)ptr, value, prev);
                *ptr = prev;
            });
        });
    }

    std::shared_ptr<Actor> SpawnCameraAndLight() {
    }

    std::shared_ptr<Actor> SpawnCamera() {

    }

    std::shared_ptr<Actor> SpawnLight(){}

    std::shared_ptr<Actor> SpawnInfinitePlane() {

    }


};
