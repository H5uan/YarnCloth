#pragma once
#include <memory>
#include <string>
#include <vector>
#include<type_traits>

#include "Component.h"
#include "Transform.h"


class Actor {
public:
    Actor();

    explicit Actor(std::string name);

    void Initialize(glm::vec3 position, glm::vec3 scale = glm::vec3(1), glm::vec3 rotation = glm::vec3(0)) const;

    void Start() const;

    void Update() const;

    void FixedUpdate() const;

    void OnDestroy() const;

    void AddComponent(const std::shared_ptr<Component> &component);

    void AddComponents(const std::initializer_list<std::shared_ptr<Component> > &newComponents);

    template<typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, T *> GetComponent() {
        T *result = nullptr;
        for (const auto &c: components) {
            result = dynamic_cast<T *>(c.get());
            if (result)
                return result;
        }
        return result;
    }

    template<typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, std::vector<T *> > GetComponents() {
        std::vector<T *> result;
        for (const auto &c: components) {
            if (auto item = dynamic_cast<T *>(c.get())) {
                result.push_back(item);
            }
        }
        return result;
    }

public:
    std::shared_ptr<Transform> transform = std::make_shared<Transform>(Transform(this));
    std::vector<std::shared_ptr<Component> > components;
    std::string name;
};
