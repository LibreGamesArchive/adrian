#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <GL/glew.h>
#include "config.h"

class ShaderProgram
{
    protected:
        GLuint m_ShaderProgram;
        GLuint m_vs;
        GLuint m_ps;
    public:
        ShaderProgram(char *vsfname, char *psfname);
        virtual ~ShaderProgram();
        virtual void InitVariables() = 0;   //initialize the shader variables
        void Enable();
        void Disable();
        static char * TextFileRead(const char *);
        void PrintLog(GLuint u);
};

#endif //__SHADER_PROGRAM_H__
