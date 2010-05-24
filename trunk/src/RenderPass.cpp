#include "globals.h"
#include "RenderPass.h"
#include "PPShaderProgram.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

static int width = hres;
static int height = vres;

RenderPass::RenderPass(ShaderProgram *s, FbType type)
{
        m_ShaderProgram = s; // initialize to zero for using fixed functionality
        m_FrameBufferObject = 0; //initialize to zero for window system provided buffer.
        
        GLenum status;
        this->type = type;

        switch(type)
        {
            case FB_DEPTH_AND_COLOR:                
                glGenTextures(1, &m_ColorTex);
                glBindTexture(GL_TEXTURE_2D, m_ColorTex);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); 
                //follow through to create the depth buffer as well.
            case FB_DEPTH_ONLY:                                           
                glGenTextures(1, &m_DepthTex);
                glBindTexture(GL_TEXTURE_2D, m_DepthTex);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	            // Remove artefact on the edges of the shadowmap
	            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);               
                                             
                if(type != FB_DEPTH_AND_COLOR)
                {
                    glDrawBuffer(GL_NONE);
	                glReadBuffer(GL_NONE);
                }  

                glGenFramebuffers(1, &m_FrameBufferObject);
                glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);  
                if(type == FB_DEPTH_AND_COLOR)  //follow through of the code from above case
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTex, 0); 
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, m_DepthTex, 0);

                status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if(status != GL_FRAMEBUFFER_COMPLETE)
                {
                    printf("Framebuffer screwed\n");
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
    glClear(GL_DEPTH_BUFFER_BIT);
    switch(type)
    {
        case FB_DEPTH_ONLY:
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        case FB_DEPTH_AND_COLOR:
        {
            glPushAttrib(GL_VIEWPORT_BIT);
            glViewport(0, 0, width, height);
            break;
        }
        default:
        case FB_NONE:
            break;
    }
    if(m_ShaderProgram)
    { 
        m_ShaderProgram->Enable();
        m_ShaderProgram->InitVariables();
    }

    //render the scene using the shaders if available
    for(GLuint i = 0; i < m_List.size(); i++)
    {
        m_List[i]->Render();
    }

    if(m_ShaderProgram)
        m_ShaderProgram->Disable();

    if(type != FB_NONE)
    {
        glPopAttrib();
        if(type == FB_DEPTH_ONLY)
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // reset the color buffer mask        
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   //switch back to window manager framebuffer.
}

RenderPass::~RenderPass()
{
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
        m_BloomShader = new PPShaderProgram((char *)"vs.txt", (char *)"bloom.txt");
        //OPENGL2.0 is supported do all the fancy stuff.
        m_isMultiPass = true;
        m_List.push_back(new RenderPass(NULL, FB_DEPTH_AND_COLOR));
        m_List.push_back(new RenderPass(m_BloomShader));

        //initialize the second pass with the fullscreen polygon.
        FullScreenPoly *tmp = new FullScreenPoly(); 
        m_List[1]->AddObject((RenderableObject*)tmp);
    }
    else
    {
        m_isMultiPass = false;
        //do stuff required for normal rendering.
        m_List.push_back(new RenderPass(NULL));
    }
}

void SceneComposer::Reset()
{
    //for(GLuint i = 0; i < m_List.size(); i++)
    {
        m_List[0]->Clear();
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

void SceneComposer::Compose(Camera *c)
{
    //for(GLuint i = 0;i < m_List.size(); i++)
    {
        c->set3DProjection();
        m_List[0]->Render();        //first pass rendered to texture

        c->set2DProjection();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_List[0]->getColorTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_List[0]->getDepthTexture());
        m_List[1]->Render();        //second pass
        glActiveTexture(GL_TEXTURE0);
    }
}
SceneComposer::~SceneComposer()
{
    if(m_BloomShader)
        delete m_BloomShader;
    for(GLuint i = 0; i < m_List.size(); i++)
    {
        delete m_List[i];
    }
    m_List.clear();
}


void FullScreenPoly::Render()
{
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);

    glTexCoord2f(1, 0);
    glVertex2f(hres, 0);

    glTexCoord2f(1, 1);
    glVertex2f(hres, vres);

    glTexCoord2f(0, 1);
    glVertex2f(0, vres);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}