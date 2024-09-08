#include "Actor.h"

#include <utility>

Actor::Actor() = default;

Actor::Actor(std::string name) : name(std::move(name)) {
}

void Actor::Initialize(const glm::vec3 position, const glm::vec3 scale, const glm::vec3 rotation) const {
    transform->position = position;
    transform->scale = scale;
    transform->rotation = rotation;
}

void Actor::Start() const {
    for (const auto &c: components) {
        c->Start();
    }
}

void Actor::AddComponent(const std::shared_ptr<Component> &component) {
    component->m_Actor = this;
    components.push_back(component);
}

void Actor::AddComponents(const std::initializer_list<std::shared_ptr<Component> > &newComponents) {
    for (const auto &c: newComponents) {
        AddComponent(c);
    }
}

void Actor::OnDestroy() const {
    for (const auto &c: components) {
        c->OnDestroy();
    }
}

void Actor::Update() const {
    for (const auto &c: components) {
        c->Update();
    }
}

void Actor::FixedUpdate() const {
    for (const auto &c: components) {
        if (c->m_IsEnabled) {
            c->FixedUpdate();
        }
    }
}
