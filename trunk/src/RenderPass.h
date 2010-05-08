#ifndef __RENDER_PASS__
#define __RENDER_PASS__

#include <GL/glew.h>
#include <vector>
using namespace std;

class RenderableObject{
public:
    virtual void Render() = 0;
};

//class for handling each pass of rendering.
class RenderPass{
    private:   
        GLuint m_ShaderProgram; //Handle for shader program to be used for this pass
        GLuint m_FrameBufferObject; //Handle for the frame bufferObject to be used for this pass;
        GLuint m_DepthTex; //Depth RenderBuffer handle.
        GLuint m_ColorTex; //Color RenderBuffer/Texture handle.
    public:
        vector <RenderableObject*> m_List;
        void AddObject(RenderableObject *obj);
        void Clear();
        RenderPass(const char *vsfname = NULL, const char *psfname = NULL);
        ~RenderPass();
        void Render();
};

class SceneComposer{
    private:
        vector <RenderPass*> m_List;
        bool m_isMultiPass;
    public:
        void addToPass(RenderableObject *obj, int index = -1); //add an object to particular index; //-1 implies all of them.
        void Reset();   //clear all the object list on all the renderpasses.
        void Compose();
        SceneComposer();
        ~SceneComposer();
};
#endif //__RENDER_PASS__
