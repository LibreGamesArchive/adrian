#include "RenderPass.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

void RenderPass::AddObject(RenderableObject *obj)
{
    m_List.push_back(obj);
}

void RenderPass::Clear()
{
    m_List.clear();
}

char * textFileRead(const char*fname)
{    
    char name[1024];
    sprintf(name, "shaders/%s",fname); 
    FILE *fp = fopen(name, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Opening(%s) failed\n", name);
		return NULL;
	}
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *buff = (char *) malloc(sizeof(char)*size+1); 
    fread(buff, sizeof(char), size, fp);
    fclose(fp);
    buff[size]='\0';
    return buff;
}

RenderPass::RenderPass(const char *vsfname, const char *psfname)
{
        m_ShaderProgram = 0; // initialize to zero for using fixed functionality
        if(vsfname && psfname)
        {
   		char *vs,*fs;
	
		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);	
	
		    vs = textFileRead(vsfname);
		    fs = textFileRead(psfname);
	
            if( vs && fs )
            {
		        const char * vv = vs;
		        const char * ff = fs;
	
		        glShaderSource(v, 1, &vv,NULL);
		        glShaderSource(f, 1, &ff,NULL);
		
		        glCompileShader(v);
		        glCompileShader(f);
	
		        m_ShaderProgram = glCreateProgram();
		
		        glAttachShader(m_ShaderProgram,v);
		        glAttachShader(m_ShaderProgram,f);
	
		        glLinkProgram(m_ShaderProgram);	

                free(vs);free(fs);
            }
        }
}

void RenderPass::Render()
{
    glUseProgram(m_ShaderProgram);
    for(GLuint i = 0; i < m_List.size(); i++)
    {
        m_List[i]->Render();
    }
    glUseProgram(0);
}

RenderPass::~RenderPass()
{
    m_List.clear();
}


SceneComposer::SceneComposer()
{
    GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		  /* Problem: glewInit failed, something is seriously wrong. */
		  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		  return;
	}
    if(GLEW_ARB_vertex_program && GLEW_ARB_fragment_program)
    {
        //OPENGL2.0 is supported do all the fancy stuff.
        m_isMultiPass = true;
        m_List.push_back(new RenderPass("vs.txt", "ps.txt"));
    }
    else
    {
        m_isMultiPass = false;
        //do stuff required for normal rendering.
        m_List.push_back(new RenderPass(NULL, NULL));
    }
}

void SceneComposer::Reset()
{
    for(GLuint i = 0; i < m_List.size(); i++)
    {
        m_List[i]->Clear();
    }
}

void SceneComposer::addToPass(RenderableObject *obj, int index)
{
    if(index < 0) //if index == -1 add the object to every render pass.
    {
        for(GLuint i = 0; i < m_List.size(); i++)
        {
            m_List[i]->AddObject(obj);
        }
    }
    else
    {
        m_List[index]->AddObject(obj);
    }
}

void SceneComposer::Compose()
{
    for(GLuint i = 0;i < m_List.size(); i++)
    {
        m_List[i]->Render();
    }
}
SceneComposer::~SceneComposer()
{
    for(GLuint i = 0; i < m_List.size(); i++)
    {
        delete m_List[i];
    }
    m_List.clear();
}
