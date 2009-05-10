#include "staticloader.h"

#include <GL/gl.h>
#include <GL/glu.h>

SModel::SModel(void)
{
	scene = NULL;
}

SModel::~SModel()
{
	if (scene)
		aiReleaseImport(scene);
}

int SModel::Load(const char *fn)
{
	scene = aiImportFile(fn,
						aiProcess_CalcTangentSpace       | 
						aiProcess_Triangulate            |
						aiProcess_JoinIdenticalVertices  |
						aiProcess_SortByPType);

	if (!scene) {
		printf("Unable to load scene!\n");
		exit(0);
	}

	return 0;
}

void SModel::Render(void)
{
static int ass = 0;
ass++;

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-angle, 1, 0, 0);

	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < (int)scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->mMeshes[i];
//		const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
		glColor3f(1-(i/18.0),1-(i/18.0),1);

		glBegin(GL_TRIANGLE_FAN);
			for (int j = 0; j < (int)mesh->mNumFaces; j++) {
				const aiFace *face = &mesh->mFaces[j];
				const aiVector3D *v = mesh->mVertices;

				for (int k = 0; k < (int)face->mNumIndices; k++) {
					unsigned int idx = face->mIndices[k];
					glVertex3f(v[idx].x, v[idx].y, v[idx].z);
					if (ass == 1) printf("(%3d %3d %3d)%f %f %f\n", i, j, k, v[idx].x, v[idx].y, v[idx].z);
				}
			}
		glEnd();
	}

	glPopMatrix();
}

