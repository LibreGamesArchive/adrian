#include "RenderPass.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>


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

static int width = 800;
static int height = 600;

RenderPass::RenderPass(const char *vsfname, const char *psfname, FbType type)
{
        m_ShaderProgram = 0; // initialize to zero for using fixed functionality
        m_FrameBufferObject = 0; //initialize to zero for window system provided buffer.

        if(vsfname && psfname)  //create the shader handle if vertex and pixel shaders are provided.
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

        GLenum status;
        this->type = type;
        switch(type)
        {
            case FB_DEPTH_AND_COLOR:                
                //follow through to create the depth buffer as well.
            case FB_DEPTH_ONLY:
                if(type != FB_DEPTH_AND_COLOR)
                {//prevent the frame buffer object generation for second time.
                    glGenFramebuffers(1, &m_FrameBufferObject);
                    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
                }

                glGenRenderbuffers(1, &m_DepthTex);
                glBindRenderbuffer(GL_RENDERBUFFER, m_DepthTex);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTex);

                glDrawBuffer(GL_NONE);
	            glReadBuffer(GL_NONE);

                status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if(status != GL_FRAMEBUFFER_COMPLETE)
                {
                    m_FrameBufferObject = 0;
                    this->type = FB_NONE;
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0); //reset before we return.
                break;            
            case FB_NONE:
            default:
                break;
        }
}

void RenderPass::AddObject(RenderableObject *obj)
{
    m_List.push_back(obj);
}

void RenderPass::Clear()
{
    m_List.clear();
}

void RenderPass::Render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject); //switch to user created frame buffer.

    if(type == FB_DEPTH_ONLY)
    {
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, width, height);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    }
    glUseProgram(m_ShaderProgram);  //use our awesome shaders.

    for(GLuint i = 0; i < m_List.size(); i++)
    {
        m_List[i]->Render();
    }
    
    glUseProgram(0);    //switch back to fixed pipeline
    if(type != FB_NONE)
    {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // reset the color buffer mask
        glPopAttrib();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   //switch back to window manager framebuffer.
}

RenderPass::~RenderPass()
{
    if(m_ShaderProgram > 0)
        glDeleteProgram(m_ShaderProgram);
    if(m_FrameBufferObject > 0)
        glDeleteFramebuffers(1, &m_FrameBufferObject);
    if(m_DepthTex > 0)
        glDeleteRenderbuffers(1, &m_DepthTex);
    if(m_ColorTex > 0)
        glDeleteTextures(1, &m_ColorTex);
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
    if(GLEW_ARB_vertex_program && GLEW_ARB_fragment_program && GL_EXT_framebuffer_object)
    {
        //OPENGL2.0 is supported do all the fancy stuff.
        m_isMultiPass = true;
        m_List.push_back(new RenderPass(NULL, NULL, FB_DEPTH_ONLY));
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
        glClear(GL_DEPTH_BUFFER_BIT);
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
