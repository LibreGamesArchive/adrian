#ifndef __SHADOW_MAP_H__
#define __SHADOW_MAP_H__

#include "ShaderProgram.h"

//currently we are assuming there is only one light source so just one mvp is sent.
class SMShaderProgram:public ShaderProgram
{
    GLfloat m_LightMatrix[16];
    GLfloat m_LightProj[16];
    GLfloat m_LightMV[16];
    GLint m_LightMVP;
    GLint m_ShadowMap;   //the shadow map texture
    GLint m_GameTexture;  //the normal texture we use only one so this works.
public:
    SMShaderProgram(char *vsname, char *fsname);
    void InitVariables();
    void Set3DProjection();
};
#endif //__SHADOW_MAP_H__