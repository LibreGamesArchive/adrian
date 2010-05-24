#ifndef __RENDER_PASS__
#define __RENDER_PASS__

#include <GL/glew.h>
#include <vector>
using namespace std;

#include "Camera.h"
#include "globals.h"
#include "ShaderProgram.h"

class RenderableObject{
public:
    virtual void Render() = 0;
};

class FullScreenPoly : public RenderableObject{   //for post processing etc.
public:
    void Render();
};

enum FbType {FB_NONE, FB_DEPTH_ONLY, FB_DEPTH_AND_COLOR};
//class for handling each pass of rendering.
class RenderPass{    
    private:   
        //GLuint m_ShaderProgram; //Handle for shader program to be used for this pass
        ShaderProgram *m_ShaderProgram;
        GLuint m_FrameBufferObject; //Handle for the frame bufferObject to be used for this pass;
        GLuint m_DepthTex; //Depth RenderBuffer handle.
        GLuint m_ColorTex; //Color RenderBuffer/Texture handle.
        FbType type; //keep track if this is depth only or not.
    public:
        vector <RenderableObject*> m_List;
        void AddObject(RenderableObject *obj);
        void Clear();
        //RenderPass(const char *vsfname = NULL, const char *psfname = NULL, FbType type = FB_NONE);
        RenderPass(ShaderProgram *s = NULL, FbType type = FB_NONE);
        ~RenderPass();
        void Render();
        void SetUniformVal(char *str, int val);
        void SetUniformVal(char *str, float val);
        GLuint getColorTexture(){return m_ColorTex;}
        GLuint getDepthTexture(){return m_DepthTex;}
};

class SceneComposer{
    private:
        vector <RenderPass*> m_RenderPass;
        bool m_isMultiPass;        
        ShaderProgram *m_BloomShader;
        ShaderProgram *m_ShadowMapShader;
    public:
        void addToPass(RenderableObject *obj, int index = -1); //add an object to particular index; //-1 implies all of them.
        void Reset();   //clear all the object list on all the renderpasses.
        void Compose(Camera *c);
        bool IsMultiPass(){return m_isMultiPass;}
        SceneComposer();
        ~SceneComposer();
};
#endif //__RENDER_PASS__
