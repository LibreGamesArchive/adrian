#include "staticloader.h"

void LoadStaticModel(const char *fn)
{
	const aiScene* scene = aiImportFile(fn,
						aiProcess_CalcTangentSpace       | 
						aiProcess_Triangulate            |
						aiProcess_JoinIdenticalVertices  |
						aiProcess_SortByPType);

	if (!scene) {
		printf("Unable to load scene!\n");
		exit(0);
	}

	/* Convert it into a Display List of whatever */
//	glBegin(GL_TRIANGLE_STRIP);
//	glEnd();
 
	aiReleaseImport(scene);
}

