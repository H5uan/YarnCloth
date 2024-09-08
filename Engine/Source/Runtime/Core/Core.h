#pragma once

#include <memory>

// TODO: change all the marcos(except condition compilation) to templates

#ifdef GLCORE_DEBUG
	#define GLCORE_ENABLE_ASSERTS
#endif

#ifdef GLCORE_ENABLE_ASSERTS
	#define GLCORE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GLCORE_ASSERT(x, ...)
#endif

constexpr unsigned int BIT(unsigned int x) {
    return 1U << x;
}

#define GLCORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
