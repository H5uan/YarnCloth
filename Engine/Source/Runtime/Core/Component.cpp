#include "Component.h"
#include "Actor.h"

std::shared_ptr<Transform> Component::GetTransform() const {
    if (m_Actor) {
        return m_Actor->transform;
    }
    return std::make_shared<Transform>(Transform(nullptr));
}
