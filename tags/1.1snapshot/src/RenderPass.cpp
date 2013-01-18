#include "globals.h"
#include "RenderPass.h"
#include "PPShaderProgram.h"
#include "SMShaderProgram.h"
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
                glBindTexture(GL_TEXTURE_2D, 0);
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
                glBindTexture(GL_TEXTURE_2D, 0);                             

                glGenFramebuffers(1, &m_FrameBufferObject);
                glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);  
                if(type == FB_DEPTH_AND_COLOR)  //follow through of the code from above case
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTex, 0); 
                else
                {               
                    glDrawBuffer(GL_NONE);
	                glReadBuffer(GL_NONE);
                }  
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
            //follow through.. 
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

void RenderPass::bindBuffer(bool flag)
{
    if(flag)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
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
	if (enable_shaders) {
		if(GLEW_ARB_vertex_program && GLEW_ARB_fragment_program && GL_EXT_framebuffer_object)
		{
			//OPENGL2.0 is supported do all the fancy stuff.
			m_isMultiPass = true;

			//create shader objects required for each pass.
			m_BloomShader = new PPShaderProgram((char *)"vs.txt", (char *)"bloom.txt"); //post proc shader.
			m_ShadowMapShader = new SMShaderProgram((char*)"smvs.txt", (char*)"smps.txt");//shadow map shader.

			//render the scene to the shadow map.
			m_RenderPass.push_back(new RenderPass(NULL, FB_DEPTH_ONLY)); 
			 //render the scene to Frame buffer to use as texture in next pass      
			m_RenderPass.push_back(new RenderPass(m_ShadowMapShader, FB_DEPTH_AND_COLOR));
			m_RenderPass.push_back(new RenderPass(m_BloomShader));

			//initialize the second pass with the fullscreen polygon.
			m_PostProc = new FullScreenPoly(); 
		}
	} else {
        m_isMultiPass = false;
        //do stuff required for normal rendering.
        m_RenderPass.push_back(new RenderPass(NULL));
    }
}

void SceneComposer::Reset()
{
    for(GLuint i = 0; i < m_RenderPass.size(); i++)
    {
        m_RenderPass[i]->Clear();
    }   //we are not clearing the last one because it is for post proc
}

void SceneComposer::addToPass(RenderableObject *obj, int index)
{
    if(index < 0) //if index == -1 add the object to every render pass.
    {
        for(GLuint i = 0; i < m_RenderPass.size(); i++)
        {
            m_RenderPass[i]->AddObject(obj);
        }
    }
    else
    {
        m_RenderPass[index]->AddObject(obj);
    }
}

#include "Sprite.h"
#include "Game.h"

void SceneComposer::Compose(Camera *c)
{
    if(m_isMultiPass)
    {   
        //add full screen poly for post proc
	    m_RenderPass[m_RenderPass.size()-1]->AddObject((RenderableObject*)m_PostProc);

        Sprite::is_bilboard = false;
        glEnable(GL_POLYGON_OFFSET_FILL);       //enable offset so there is no z fighting.
        glPolygonOffset(1.0, 1.0);
        ((SMShaderProgram*)m_ShadowMapShader)->Set3DProjection();   //this informatioin should be moved to RenderPass
        m_RenderPass[0]->Render();        //first pass for shadow map generation
        glDisable(GL_POLYGON_OFFSET_FILL);

        GLuint size = m_RenderPass.size();
        c->set3DProjection();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_RenderPass[0]->getDepthTexture()); //last but one pass
        glActiveTexture(GL_TEXTURE0);
        m_RenderPass[size-2]->Render();        //second pass rendered to texture
        //-------------------- hack to stop the sprites' shadows from rotating.
        Sprite::is_bilboard = true;
        m_RenderPass[size-2]->bindBuffer();
        if(game->initialized){
            game->map->TransparentRender();
        }
        m_RenderPass[size-2]->bindBuffer(false);
        //---------------------end hack

        c->set2DProjection();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RenderPass[size-2]->getColorTexture());  //last but one pass
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_RenderPass[size-2]->getDepthTexture()); //last but one pass
        m_RenderPass[size-1]->Render();        //last pass
        glActiveTexture(GL_TEXTURE0);
    }
    else
    {
        c->set3DProjection();
        m_RenderPass[0]->Render();
    }
}
SceneComposer::~SceneComposer()
{
    if(m_isMultiPass)
    {
        delete m_BloomShader;
        delete m_ShadowMapShader;
        delete m_PostProc;
    }
    for(GLuint i = 0; i < m_RenderPass.size(); i++)
    {
        delete m_RenderPass[i];
    }
    m_RenderPass.clear();
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
