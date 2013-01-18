#include "PPShaderProgram.h"
#include <cstdio>

PPShaderProgram::PPShaderProgram(char *vsfname, char *fsfname)
    :ShaderProgram(vsfname, fsfname)
{
}

//Initialize the variables requried by the Post Proc shader Program
void PPShaderProgram::InitVariables()
{
    ///the program has to be enable before a call is made to initialize
    GLint ctex_loc = glGetUniformLocation(m_ShaderProgram, "tex0");
    if(ctex_loc == GL_INVALID_OPERATION)
        printf("Could not get uniformloc\n");
    glUniform1i(ctex_loc, 0);   //first texture unit is color

    GLint dtex_loc = glGetUniformLocation(m_ShaderProgram, "tex1");
    if(dtex_loc == GL_INVALID_OPERATION)
        printf("Could not get uniformloc\n");
    glUniform1i(dtex_loc, 1);   //second texture unit is for depth
}
