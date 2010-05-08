#include "RenderPass.h"


void RenderPass::AddObject(RenderableObject *obj)
{
    m_List.push_back(obj);
}

void RenderPass::Clear()
{
    m_List.clear();
}

RenderPass::RenderPass(char *vsfname, char *psfname)
{
}


SceneComposer::SceneComposer()
{
    glewInit();
    if(GLEW_VERSION_2_0)
    {
        //OPENGL2.0 is supported do all the fancy stuff.
        m_isMultiPass = true;
    }
    else
    {
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