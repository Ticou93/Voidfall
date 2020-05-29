#include "GlLoader.hpp"

#include <cassert>


PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERSOURCEPROC glGetShaderInfo = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;

#define LOAD_GL_FUNCTION(type, name) \
    (name) = (PFN##type##PROC) glXGetProcAddress((unsigned char *) #name); \
    assert(name && "GL function not found");

void LoadGlFunctions()
{
	LOAD_GL_FUNCTION(GLCREATESHADER, glCreateShader)
		LOAD_GL_FUNCTION(GLSHADERSOURCE, glShaderSource)
		LOAD_GL_FUNCTION(GLCOMPILESHADER, glCompileShader)
		LOAD_GL_FUNCTION(GLGETSHADERIV, glGetShaderiv)
		LOAD_GL_FUNCTION(GLGETSHADERSOURCE, glGetShaderInfo)
		LOAD_GL_FUNCTION(GLGETSHADERINFOLOG, glGetShaderInfoLog)
		LOAD_GL_FUNCTION(GLCREATEPROGRAM, glCreateProgram)
		LOAD_GL_FUNCTION(GLATTACHSHADER, glAttachShader)
		LOAD_GL_FUNCTION(GLLINKPROGRAM, glLinkProgram)
		LOAD_GL_FUNCTION(GLGETPROGRAMIV, glGetProgramiv)
		LOAD_GL_FUNCTION(GLGETPROGRAMINFOLOG, glGetProgramInfoLog)
		LOAD_GL_FUNCTION(GLDELETESHADER, glDeleteShader)
		LOAD_GL_FUNCTION(GLGENBUFFERS, glGenBuffers)
		LOAD_GL_FUNCTION(GLBINDBUFFER, glBindBuffer)
		LOAD_GL_FUNCTION(GLBUFFERDATA, glBufferData)
		LOAD_GL_FUNCTION(GLGENVERTEXARRAYS, glGenVertexArrays)
		LOAD_GL_FUNCTION(GLBINDVERTEXARRAY, glBindVertexArray)
		LOAD_GL_FUNCTION(GLVERTEXATTRIBPOINTER, glVertexAttribPointer)
		LOAD_GL_FUNCTION(GLENABLEVERTEXATTRIBARRAY, glEnableVertexAttribArray)
		LOAD_GL_FUNCTION(GLUSEPROGRAM, glUseProgram)
		LOAD_GL_FUNCTION(GLGETUNIFORMLOCATION, glGetUniformLocation)
		LOAD_GL_FUNCTION(GLUNIFORMMATRIX4FV, glUniformMatrix4fv)
		LOAD_GL_FUNCTION(GLUNIFORM3FV, glUniform3fv)
}