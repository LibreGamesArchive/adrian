#include "globals.h"
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

static int width = hres;
static int height = vres;

void printLog(GLuint v)
{
    GLint blen = 0;	
    GLsizei slen = 0;

    glGetShaderiv(v, GL_INFO_LOG_LENGTH , &blen);   
    if (blen > 1)
    {
        GLchar* compiler_log = (GLchar*)malloc(blen);
        glGetInfoLogARB(v, blen, &slen, compiler_log);
        printf("Error: %s\n", compiler_log);
        free (compiler_log);
    }
}

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

                GLint vsstatus, fsstatus;
                
		        glCompileShader(v);
                glGetShaderiv(v, GL_COMPILE_STATUS, &vsstatus);

		        glCompileShader(f);
                glGetShaderiv(f, GL_COMPILE_STATUS, &fsstatus);
	
                if (fsstatus && vsstatus)
                {                  
		            m_ShaderProgram = glCreateProgram();
		
		            glAttachShader(m_ShaderProgram,v);
		            glAttachShader(m_ShaderProgram,f);
	
		            glLinkProgram(m_ShaderProgram);	
                }
                else
                {     
                    printLog(v);
                    printLog(f);
                }
                free(vs);free(fs);
            }
        }

        GLenum status;
        this->type = type;
        switch(type)
        {
            case FB_DEPTH_AND_COLOR:                
                glGenFramebuffers(1, &m_FrameBufferObject);
                glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);

                glGenTextures(1, &m_ColorTex);
                glBindTexture(GL_TEXTURE_2D, m_ColorTex);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); 

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTex, 0); 
                //follow through to create the depth buffer as well.
                glGenTextures(1, &m_ColorTex);
                glBindTexture(GL_TEXTURE_2D, m_ColorTex);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	            // Remove artefact on the edges of the shadowmap
	            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);    
                glGenerateMipmapEXT(GL_TEXTURE_2D);
                //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, m_ColorTex, 0);

            case FB_DEPTH_ONLY:                                           
                /*glGenRenderbuffers(1, &m_DepthTex);
                glBindRenderbuffer(GL_RENDERBUFFER, m_DepthTex);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTex);*/
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
                if(type != FB_DEPTH_AND_COLOR)
                {
                    glDrawBuffer(GL_NONE);
	                glReadBuffer(GL_NONE);
                }
                else
                {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTex, 0);
                }
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, m_DepthTex, 0);
                //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTex);
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

void RenderPass::SetUniformVars(GLuint colorid, GLuint depthid)
{
    //we can use the textures only if we have some program loaded to use them.
    if(m_ShaderProgram)
    {
            glUseProgram(m_ShaderProgram);
            GLint ctex_loc = glGetUniformLocation(m_ShaderProgram, "tex0");
            if(ctex_loc == GL_INVALID_OPERATION)
                printf("Could not get uniformloc\n");
            glUniform1i(ctex_loc, colorid);

            GLint dtex_loc = glGetUniformLocation(m_ShaderProgram, "tex1");
            if(dtex_loc == GL_INVALID_OPERATION)
                printf("Could not get uniformloc\n");
            glUniform1i(dtex_loc, depthid);
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
    glUseProgram(m_ShaderProgram);  //use our awesome shaders.

    for(GLuint i = 0; i < m_List.size(); i++)
    {
        m_List[i]->Render();
    }
    glUseProgram(0);    //switch back to fixed pipeline
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
        m_List.push_back(new RenderPass(NULL, NULL, FB_DEPTH_AND_COLOR));
        m_List.push_back(new RenderPass("vs.txt", "bloom.txt"));
        FullScreenPoly *tmp = new FullScreenPoly();
        m_List[1]->AddObject((RenderableObject*)tmp);
    //    m_List[1]->SetUniformVars(m_List[0]->getColorTexture(), m_List[1]->getDepthTexture());
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
     //   glClear(GL_DEPTH_BUFFER_BIT);
        c->set3DProjection();
        m_List[0]->Render();        //first pass rendered to texture

       // glClear(GL_COLOR_BUFFER_BIT);
        c->set2DProjection();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_List[0]->getColorTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_List[0]->getDepthTexture());
        m_List[1]->SetUniformVars(0, 1);
        m_List[1]->Render();        //second pass
        glActiveTexture(GL_TEXTURE0);
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
