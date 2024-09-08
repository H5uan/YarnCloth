#include "RenderingCommand.h"

#include "Platform/OpenGL/OpenGLRenderingAPI.h"

#include <Core/Base.h>


Scope<RenderingAPI> RenderCommand::sRendererAPI = CreateScope<OpenGLRenderingAPI>();
