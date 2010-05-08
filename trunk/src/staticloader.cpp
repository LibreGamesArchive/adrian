#include <stdio.h>
#include "staticloader.h"

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
	
	/* Load into OpenGL */
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColor3f(1.0,1.0,1.0);
	for (int i = 0; i < (int)scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->mMeshes[i];
		const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
//		glColor3f(1-(i/18.0),1-(i
//		/18.0),1);
		aiColor4D diffuseColor;
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS) {
			float f[4];
			f[0] = diffuseColor.r;
			f[1] = diffuseColor.g;
			f[2] = diffuseColor.b;
			f[3] = diffuseColor.a;
//			glLightfv(GL_LIGHT0, GL_DIFFUSE, f);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, f);
		}

		aiColor4D specularColor;
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specularColor) == AI_SUCCESS) {
			float f[4];
			f[0] = specularColor.r;
			f[1] = specularColor.g;
			f[2] = specularColor.b;
			f[3] = specularColor.a;
//			glLightfv(GL_LIGHT0, GL_SPECULAR, f);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f);
		}

		aiColor4D ambColor;
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambColor) == AI_SUCCESS) {
			float f[4];
			f[0] = ambColor.r;
			f[1] = ambColor.g;
			f[2] = ambColor.b;
			f[3] = ambColor.a;
//			glLightfv(GL_LIGHT0, GL_AMBIENT, f);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, f);
		}

		aiColor4D emmColor;
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &emmColor) == AI_SUCCESS) {
			float f[4];
			f[0] = emmColor.r;
			f[1] = emmColor.g;
			f[2] = emmColor.b;
			f[3] = emmColor.a;
//			glLightfv(GL_LIGHT0, GL_EMISSIVE, f);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, f);
		}

		float shin;
		if (aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shin) == AI_SUCCESS) {
			printf("SHIN=%f\n", shin);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shin);
		}


		glBegin(GL_TRIANGLE_FAN);
			for (int j = 0; j < (int)mesh->mNumFaces; j++) {
				const aiFace *face = &mesh->mFaces[j];
				const aiVector3D *v = mesh->mVertices;

				for (int k = 0; k < (int)face->mNumIndices; k++) {
					unsigned int idx = face->mIndices[k];
					glVertex3f((v[idx].x - xoff)/scalefactor,
							   (v[idx].y - yoff)/scalefactor,
							   (v[idx].z - zoff)/scalefactor);
//					printf("(%3d %3d %3d)%f %f %f\n", i, j, k, v[idx].x, v[idx].y, v[idx].z);
				}
			}
		glEnd();
	}

	glEndList();

	return 0;
}

void SModel::Render(void)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-angle, 1, 0, 0);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

//	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glCallList(list);
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}

