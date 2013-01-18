#include "SMShaderProgram.h"
#include "globals.h"
#include <cstdio>

SMShaderProgram::SMShaderProgram(char *vsname, char *fsname)
:ShaderProgram(vsname, fsname)
{
    ///the program has to be enable before a call is made to initialize
    this->Enable();        
    m_ShadowMap = glGetUniformLocation(m_ShaderProgram, "ShadowMap");
    if(m_ShadowMap == GL_INVALID_OPERATION)
        printf("Could not get ShadowMap\n");
   
    m_GameTexture = glGetUniformLocation(m_ShaderProgram, "GameTexture");
    if(m_GameTexture == GL_INVALID_OPERATION)
        printf("Could not get GameTexture\n");

    m_LightMVP = glGetUniformLocation(m_ShaderProgram, "LightMVP");
    if(m_LightMVP == GL_INVALID_OPERATION)
    {
       printf("Could nto get the LightMVP\n");
    }
    //light projection setting.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2000, 2000, -400, 2000, -2000, 2000);        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-800, 30, 800, -780, 0, 780, 0, 1, 0);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_LightMV);
    glGetFloatv(GL_PROJECTION_MATRIX, m_LightProj);

    glLoadIdentity();
    glTranslatef(0.5, 0.5, 0.5);
    glScalef(0.5, 0.5, 0.5);    
    glMultMatrixf(m_LightProj);
    glMultMatrixf(m_LightMV);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_LightMatrix);
    glLoadIdentity();
    glMultMatrixf(m_LightMV);   //reverting to whatever was there in Modelview matrix.*/

    this->Disable();
}

void SMShaderProgram::InitVariables()
{
    glUniform1i(m_GameTexture, 0);   //first texture unit is color
    glUniform1i(m_ShadowMap, 1);   //second texture for the lightmap
    glUniformMatrix4fv(m_LightMVP, 1, GL_FALSE, m_LightMatrix); //we can send more matrices each for one light.
}

void SMShaderProgram::Set3DProjection() //to ensure that we use the lights projection before rendering to shadow map.
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(m_LightProj);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(m_LightMV);
}
