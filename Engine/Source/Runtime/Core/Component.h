#pragma once

#include <iostream>
#include <string>

#include <Core/Transform.h>

class Actor;

class Component {
public:
    virtual ~Component() = default;

    virtual void Start() = 0;

    virtual void Update() = 0;

    virtual void FixedUpdate() = 0;

    virtual void OnDestroy() = 0;

    std::shared_ptr<Transform> GetTransform() const;

    std::string name = "Component";

    Actor *m_Actor = nullptr;

    bool m_IsEnabled = true;
};
