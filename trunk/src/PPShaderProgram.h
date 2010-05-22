#ifndef __POSTPROC_SHADER_PROGRAM_H__
#define __POSTPROC_SHADER_PROGRAM_H__

#include "ShaderProgram.h"

//class to handle all the post processing shaders 
//they take two textures: color and depth to operate
class PPShaderProgram:public ShaderProgram
{
    public:
        PPShaderProgram(char *vsfname, char *fsfname);
        void InitVariables();
};

#endif //__POSTPROC_SHADER_PROGRAM_H__
