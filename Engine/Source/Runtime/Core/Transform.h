#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Actor;

class Transform {
public:
    explicit Transform(Actor *actor) {
        m_actor = actor;
    }

    [[nodiscard]] glm::mat4 matrix() const {
        glm::mat4 result = glm::mat4(1.0f);
        result = glm::translate(result, position);
        result = glm::rotate(result, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        result = glm::rotate(result, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        result = glm::rotate(result, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        result = glm::scale(result, scale);
        return result;
    }

    [[nodiscard]] Actor *actor() const {
        return m_actor;
    }

    void Reset() {
        position = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
    }

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    Actor *m_actor;
};
