#pragma once

#include <GL/glew.h>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#define ASSERT(x) if(!(x)) __builtin_trap();
#define GLCALL(x) GLClearError(); x;  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();


bool GLLogCall(const char* function, const char* file, int line);



class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& Shader) const ;

};