#include "ShaderProgram.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

ShaderProgram::ShaderProgram(char *vsfname, char* psfname)
{
    //load the shader programs from these files and create a program handle. 
    if(vsfname && psfname)  //create the shader handle if vertex and pixel shaders are provided.
    {
        char *vs,*fs;

        m_vs = glCreateShader(GL_VERTEX_SHADER);
        m_ps = glCreateShader(GL_FRAGMENT_SHADER);	

        vs = TextFileRead(vsfname);
        fs = TextFileRead(psfname);

        if( vs && fs )
        {
            const char * vv = vs;
            const char * ff = fs;

            glShaderSource(m_vs, 1, &vv,NULL);
            glShaderSource(m_ps, 1, &ff,NULL);

            GLint vsstatus, fsstatus;

            glCompileShader(m_vs);
            glGetShaderiv(m_vs, GL_COMPILE_STATUS, &vsstatus);

            glCompileShader(m_ps);
            glGetShaderiv(m_ps, GL_COMPILE_STATUS, &fsstatus);

            if (fsstatus && vsstatus)
            {                  
                m_ShaderProgram = glCreateProgram();

                glAttachShader(m_ShaderProgram, m_vs);
                glAttachShader(m_ShaderProgram, m_ps);

                glLinkProgram(m_ShaderProgram);	
            }
            else
            {     
                PrintLog(m_vs);
                PrintLog(m_ps);
            }
            free(vs);free(fs);
        }
    }
}

ShaderProgram::~ShaderProgram()
{
    //release the handles of the program, shaders
    glDetachShader(m_ShaderProgram, m_vs);
    glDetachShader(m_ShaderProgram, m_ps);
    glDeleteShader(m_vs);
    glDeleteShader(m_ps);
    glDeleteProgram(m_ShaderProgram);
}

char * ShaderProgram::TextFileRead(const char*fname)
{    
    char name[1024];
    sprintf(name, "%s/shaders/%s", GAME_DATA_PATH, fname); 
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

void ShaderProgram::PrintLog(GLuint v)
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

void ShaderProgram::Enable()
{
    glUseProgram(m_ShaderProgram);
}

void ShaderProgram::Disable()
{
    glUseProgram(0); //setting program as 0 switches to fixed functionality
}
