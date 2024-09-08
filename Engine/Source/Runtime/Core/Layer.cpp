#include "glpch.h"
#include "Layer.h"

#include <utility>

namespace GLCore {
    Layer::Layer(std::string debug_name)
        : m_DebugName(std::move(debug_name)) {
    }
}
