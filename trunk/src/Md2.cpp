#include "Md2.h"
#include <string.h>
#include <SDL/SDL_image.h>
#include <ctype.h>
#include "tga.h"


static void swap(unsigned char & a, unsigned char & b)
{
    unsigned char temp;
	
	temp = a;
	a    = b;
	b    = temp;

    return;
}


CLoadMD2::CLoadMD2()
{
    // Here we initialize our structures to 0
    memset(&m_Header, 0, sizeof(tMd2Header));

    // Set the pointers to null
    m_pSkins=NULL;
    m_pTexCoords=NULL;
    m_pTriangles=NULL;
    m_pFrames=NULL;
}


///////////////////////////////// IMPORT MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This is called by the client to open the .Md2 file, read it, then clean up
/////
///////////////////////////////// IMPORT MD2 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CLoadMD2::ImportMD2(t3DModel *pModel, char *strFileName, char *strTexture)
{
    char strMessage[255] = {0};

    // Open the MD2 file in binary
    m_FilePointer = fopen(strFileName, "rb");

    // Make sure we have a valid file pointer (we found the file)
    if(!m_FilePointer) 
    {
        // Display an error message and don't load anything if no file was found
        sprintf(strMessage, "Unable to find the file: %s!", strFileName);
        printf("%s\n", strMessage );
        Quit(1);
    }
    
    // Read the header data and store it in our m_Header member variable
    fread(&m_Header, 1, sizeof(tMd2Header), m_FilePointer);

    // Make sure the version is this crazy number '8' or else it's a bad egg
    if(m_Header.version != 8)
    {
        // Display an error message for bad file format, then stop loading
        sprintf(strMessage, "Invalid file format (Version not 8): %s!", strFileName);
        printf("%s\n", strMessage );
        Quit(1);
    }

    // Read in the model and animation data
    ReadMD2Data();
    
    // Here we pass in our model structure to it can store the read Quake data
    // in our own model and object structure data
    ConvertDataStructures(pModel);

    // If there is a valid texture name passed in, we want to set the texture data
    if(strTexture)
    {
        // Create a local material info structure
        tMaterialInfo texture;

        // Copy the name of the file into our texture file name variable
        strcpy(texture.strFile, strTexture);

        // Since there is only one texture for a .Md2 file, the ID is always 0
        texture.texureId = 0;

        // The tile or scale for the UV's is 1 to 1 (but Quake saves off a 0-256 ratio)
        texture.uTile = texture.uTile = 1;

        // We only have 1 material for a model
        pModel->numOfMaterials = 1;

        // Add the local material info to our model's material list
        pModel->pMaterials.push_back(texture);
    }

    // Clean up after everything
    CleanUp();

    // Return a success
    return true;
}


///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function reads in all of the model's data, except the animation frames
/////
///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ReadMD2Data()
{
    // Create a larger buffer for the frames of animation (not fully used yet)
    unsigned char buffer[MD2_MAX_FRAMESIZE];

    // Here we allocate all of our memory from the header's information
    m_pSkins     = new tMd2Skin [m_Header.numSkins];
    m_pTexCoords = new tMd2TexCoord [m_Header.numTexCoords];
    m_pTriangles = new tMd2Face [m_Header.numTriangles];
    m_pFrames    = new tMd2Frame [m_Header.numFrames];

    // Next, we start reading in the data by seeking to our skin names offset
    fseek(m_FilePointer, m_Header.offsetSkins, SEEK_SET);
    
    // Depending on the skin count, we read in each skin for this model
    fread(m_pSkins, sizeof(tMd2Skin), m_Header.numSkins, m_FilePointer);
    
    // Move the file pointer to the position in the file for texture coordinates
    fseek(m_FilePointer, m_Header.offsetTexCoords, SEEK_SET);
    
    // Read in all the texture coordinates in one fell swoop
    fread(m_pTexCoords, sizeof(tMd2TexCoord), m_Header.numTexCoords, m_FilePointer);

    // Move the file pointer to the triangles/face data offset
    fseek(m_FilePointer, m_Header.offsetTriangles, SEEK_SET);
    
    // Read in the face data for each triangle (vertex and texCoord indices)
    fread(m_pTriangles, sizeof(tMd2Face), m_Header.numTriangles, m_FilePointer);
            
    // Move the file pointer to the vertices (frames)
    fseek(m_FilePointer, m_Header.offsetFrames, SEEK_SET);


    for (int i=0; i < m_Header.numFrames; i++)
    {
        tMd2AliasFrame *pFrame = (tMd2AliasFrame *) buffer;

        m_pFrames[i].pVertices = new tMd2Triangle [m_Header.numVertices];

        fread(pFrame, 1, m_Header.frameSize, m_FilePointer);

        strcpy(m_pFrames[i].strName, pFrame->name);
            
        tMd2Triangle *pVertices = m_pFrames[i].pVertices;

        for (int j=0; j < m_Header.numVertices; j++)
        {
            pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
            pVertices[j].vertex[2] = -1 * (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]);
            pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
        }
    }
}


///////////////////////////////// PARSE ANIMATIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function fills in the animation list for each animation by name and frame
/////
///////////////////////////////// PARSE ANIMATIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ParseAnimations(t3DModel *pModel)
{
    tAnimationInfo animation;
    string strLastName = "";

    // Go through all of the frames of animation and parse each animation
    for(int i = 0; i < pModel->numOfObjects; i++)
    {
        // Assign the name of this frame of animation to a string object
        string strName  = m_pFrames[i].strName;
        int frameNum = 0;
        
        // Go through and extract the frame numbers and erase them from the name
        for(int j = 0; j < strName.length(); j++)
        {
            // If the current index is a number and it's one of the last 2 characters of the name
            if( isdigit(strName[j]) && j >= strName.length() - 2)
            {
                // Use a C function to convert the character to a integer.
                // Notice we use the address to pass in the current character and on
                frameNum = atoi(&strName[j]);

                // Erase the frame number from the name so we extract the animation name
                strName.erase(j, strName.length() - j);
                break;
            }
        }

        // Check if this animation name is not the same as the last frame,
        // or if we are on the last frame of animation for this model
        if(strName != strLastName || i == pModel->numOfObjects - 1)
        {
            // If this animation frame is NOT the first frame
            if(strLastName != "")
            {
                // Copy the last animation name into our new animation's name
                strcpy(animation.strName, strLastName.c_str());

                // Set the last frame of this animation to i
                animation.endFrame = i;

                // Add the animation to our list and reset the animation object for next time
                pModel->pAnimations.push_back(animation);
                memset(&animation, 0, sizeof(tAnimationInfo));

                // Increase the number of animations for this model
                pModel->numOfAnimations++;
            }

            // Set the starting frame number to the current frame number we just found,
            // minus 1 (since 0 is the first frame) and add 'i'.
            animation.startFrame = frameNum - 1 + i;
        }

        // Store the current animation name in the strLastName string to check it latter
        strLastName = strName;
    }
}


///////////////////////////////// CONVERT DATA STRUCTURES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function converts the .md2 structures to our own model and object structures
/////
///////////////////////////////// CONVERT DATA STRUCTURES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ConvertDataStructures(t3DModel *pModel)
{
    int j = 0, i = 0;

    // Initialize the model structure first before assigning data to it
    memset(pModel, 0, sizeof(t3DModel));
    
    // Set the number of objects to our number of frames since pObjects will
    // hold the key frames
    pModel->numOfObjects = m_Header.numFrames;

    // Create our animation list and store it in our model
    ParseAnimations(pModel);

    // Go through every key frame and store it's vertices info in our pObject list.
    for (i=0; i < pModel->numOfObjects; i++)
    {
        // Create a local object to store the first frame of animation's data
        t3DObject currentFrame = {0};

        // Assign the vertex, texture coord and face count to our new structure
        currentFrame.numOfVerts   = m_Header.numVertices;
        currentFrame.numTexVertex = m_Header.numTexCoords;
        currentFrame.numOfFaces   = m_Header.numTriangles;

        // Allocate memory for the vertices, texture coordinates and face data.
        currentFrame.pVerts    = new CVector3 [currentFrame.numOfVerts];

        // Go through all of the vertices and assign them over to our structure
        for (j=0; j < currentFrame.numOfVerts; j++)
        {
            currentFrame.pVerts[j].x = m_pFrames[i].pVertices[j].vertex[0];
            currentFrame.pVerts[j].y = m_pFrames[i].pVertices[j].vertex[1];
            currentFrame.pVerts[j].z = m_pFrames[i].pVertices[j].vertex[2];
        }

         delete m_pFrames[i].pVertices;

        // Check if we are past the first key frame
        if(i > 0)
        {
            // Here we add the current object (or frame) to our list object list
            pModel->pObject.push_back(currentFrame);
            continue;   // Go on to the next key frame
        }
    
        // We will only get here ONE because we just need this information
        // calculated for the first key frame.
            
        // Allocate memory for our UV coordinates and face information
        currentFrame.pTexVerts = new CVector2 [currentFrame.numTexVertex];
        currentFrame.pFaces    = new tFace [currentFrame.numOfFaces];

        // Go through all of the uv coordinates and assign them over to our structure.
        // The UV coordinates are not normal uv coordinates, they have a pixel ratio of
        // 0 to 256.  We want it to be a 0 to 1 ratio, so we divide the u value by the
        // skin width and the v value by the skin height.  This gives us our 0 to 1 ratio.
        // For some reason also, the v coodinate is flipped upside down.  We just subtract
        // the v coordinate from 1 to remedy this problem.
        for(j=0; j < currentFrame.numTexVertex; j++)
        {
            currentFrame.pTexVerts[j].x = m_pTexCoords[j].u / float(m_Header.skinWidth);
            currentFrame.pTexVerts[j].y = 1 - m_pTexCoords[j].v / float(m_Header.skinHeight);
        }

        // Go through all of the face data and assign it over to OUR structure
        for(j=0; j < currentFrame.numOfFaces; j++)
        {
            // Assign the vertex indices to our face data
            currentFrame.pFaces[j].vertIndex[0] = m_pTriangles[j].vertexIndices[0];
            currentFrame.pFaces[j].vertIndex[1] = m_pTriangles[j].vertexIndices[1];
            currentFrame.pFaces[j].vertIndex[2] = m_pTriangles[j].vertexIndices[2];

            // Assign the texture coord indices to our face data
            currentFrame.pFaces[j].coordIndex[0] = m_pTriangles[j].textureIndices[0];
            currentFrame.pFaces[j].coordIndex[1] = m_pTriangles[j].textureIndices[1];
            currentFrame.pFaces[j].coordIndex[2] = m_pTriangles[j].textureIndices[2];
        }

        // Here we add the current object (or frame) to our list object list
        pModel->pObject.push_back(currentFrame);
    }
}

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function cleans up our allocated memory and closes the file
/////
///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::CleanUp()
{
    // This just just the regular cleanup or our md2 model class.  We can free
    // all of this data because we already have it stored in our own structures.

    fclose(m_FilePointer);                      // Close the current file pointer

    if(m_pSkins)     delete [] m_pSkins;        // Free the skins data
    if(m_pTexCoords) delete m_pTexCoords;       // Free the texture coord data
    if(m_pTriangles) delete m_pTriangles;       // Free the triangle face data
    if(m_pFrames)    delete m_pFrames;          // Free the frames of animation
}



// class for handling Md2 objects
Md2::Md2 ( char *filename)
{
	bzero( this->filename, FILE_NAME_SIZE );
	strcpy( this->filename, filename );
	//strcpy( TEXTURE_NAME, texFileName );
	bzero( TEXTURE_NAME, 1024 );
	strcpy( TEXTURE_NAME, filename );
	strncat( TEXTURE_NAME, "/model.bmp", strlen("/model.bmp") );
	strncat( this->filename, "/tris.md2", strlen( "/tris.md2" ) );

	char AnimNum = 1;
 	//fscanf( fp, "%d", &AnimNum );

//	g_Texture[0] = texid;
	lastTime = 0.0f;
	x = 0;
	y = 25;
	z = 0;

	g_ViewMode = GL_TRIANGLES;
	g_LoadMd2.ImportMD2(&g_3DModel, this->filename, TEXTURE_NAME);

    // There is no color information for these models, as well as only one
    // texture.  If everything goes well, it should load the TEXTURE_NAME file.

    // Go through all the materials
    for(int i = 0; i < g_3DModel.numOfMaterials; i++)
    {
        // Check to see if there is a file name to load in this material
        if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
        {
            // Use the name of the texture file to load the bitmap, with a texture ID (i).
            // We pass in our global texture array, the name of the texture, and an ID to reference it. 
               createTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);
//				loadTGA( g_3DModel.pMaterials[i].strFile, g_Texture[i] ); 
        }

        // Set the texture ID for this material
        g_3DModel.pMaterials[i].texureId = i;
    }

	//initialization for the animation....
	g_3DModel.currentAnim = AnimNum % (g_3DModel.numOfAnimations);
       // Set the current frame to be the starting frame of the new animation
	g_3DModel.currentFrame = g_3DModel.pAnimations[g_3DModel.currentAnim].startFrame;


    //glEnable(GL_CULL_FACE);                             // Turn culling on
    //glCullFace(GL_FRONT);                               // Quake2 uses front face culling apparently

	//knowing the animations....
        for ( int i = 0 ; i < g_3DModel.numOfAnimations; i++ )
        {
                if( (strcmp( g_3DModel.pAnimations[i].strName,"stand") == 0 )|| (strcmp(g_3DModel.pAnimations[i].strName,"stand0") == 0 ))
                {
                        AnimNum_Stand = i;
                }
                else if(  strcmp( g_3DModel.pAnimations[i].strName,"attack") == 0 )
                {
                        AnimNum_Attack = i;
                }
                else if(  strcmp( g_3DModel.pAnimations[i].strName,"run") == 0 || strcmp( g_3DModel.pAnimations[i].strName,"run0") == 0 )
                {
                        AnimNum_Run = i;
                }
                else if(  (strcmp(g_3DModel.pAnimations[i].strName,"death")==0) || (strcmp(g_3DModel.pAnimations[i].strName,"deatha")==0) || strcmp( g_3DModel.pAnimations[i].strName,"death1") == 0 || strcmp( g_3DModel.pAnimations[i].strName,"death0") == 0 )
                {
                        AnimNum_Death = i;
                }
        }
} 

void Md2::render( void )
{
	glEnable( GL_TEXTURE_2D );
    glPushMatrix( );
//    glScalef( scale[0], scale[1], scale[2] );
    glTranslatef( x, y, z );
 //   glRotatef( rot_final, rot_ori[0], rot_ori[1], rot_ori[2] );
	glRotatef( -facingAngle , 0 , 1 , 0 );
	glBindTexture( GL_TEXTURE_2D, g_Texture[0] );
	AnimateMD2Model(&g_3DModel);
    glPopMatrix( );
	glDisable( GL_TEXTURE_2D );
}

void Md2::AnimateMD2Model( t3DModel *pModel )
{
    // Make sure we have valid objects just in case. (size() is in the vector class)
    if(pModel->pObject.size() <= 0) return;

    // Here we grab the current animation that we are on from our model's animation list
    tAnimationInfo *pAnim = &(pModel->pAnimations[pModel->currentAnim]);

    // This gives us the current frame we are on.  We mod the current frame plus
    // 1 by the current animations end frame to make sure the next frame is valid.
    // If the next frame is past our end frame, then we go back to zero.  We check this next.
    int nextFrame = (pModel->currentFrame + 1) % pAnim->endFrame;

    // If the next frame is zero, that means that we need to start the animation over.
    // To do this, we set nextFrame to the starting frame of this animation.
    if(nextFrame == 0)
        nextFrame =  pAnim->startFrame;

    // Get the current key frame we are on
    t3DObject *pFrame =      &pModel->pObject[pModel->currentFrame];

    // Get the next key frame we are interpolating too
    t3DObject *pNextFrame =  &pModel->pObject[nextFrame];

    // Get the first key frame so we have an address to the texture and face information
    t3DObject *pFirstFrame = &pModel->pObject[0];

    // Next, we want to get the current time that we are interpolating by.  Remember,
    // if t = 0 then we are at the beginning of the animation, where if t = 1 we are at the end.
    // Anyhing from 0 to 1 can be thought of as a percentage from 0 to 100 percent complete.
    float t = ReturnCurrentTime(pModel, nextFrame);

    // Start rendering lines or triangles, depending on our current rendering mode (Lft Mouse Btn)
    glBegin(g_ViewMode);

        // Go through all of the faces (polygons) of the current frame and draw them
        for(int j = 0; j < pFrame->numOfFaces; j++)
        {
            // Go through each corner of the triangle and draw it.
            for(int whichVertex = 0; whichVertex < 3; whichVertex++)
            {
                // Get the index for each point of the face
                int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];

                // Get the index for each texture coordinate for this face
                int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];

                // Make sure there was a UVW map applied to the object.  Notice that
                // we use the first frame to check if we have texture coordinates because
                // none of the other frames hold this information, just the first by design.
                if(pFirstFrame->pTexVerts)
                {
                    // Pass in the texture coordinate for this vertex
                    glTexCoord2f(pFirstFrame->pTexVerts[ texIndex ].x,
                                 pFirstFrame->pTexVerts[ texIndex ].y);
                }

                // Now we get to the interpolation part! (*Bites his nails*)
                // Below, we first store the vertex we are working on for the current
                // frame and the frame we are interpolating too.  Next, we use the
                // linear interpolation equation to smoothly transition from one
                // key frame to the next.

                // Store the current and next frame's vertex
                CVector3 vPoint1 = pFrame->pVerts[ vertIndex ];
                CVector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];

                // By using the equation: p(t) = p0 + t(p1 - p0), with a time t
                // passed in, we create a new vertex that is closer to the next key frame.
		 glVertex3f(vPoint1.x + 0.01 * (vPoint2.x - vPoint1.x), // Find the interpolated X
                           vPoint1.y + 0.01 * (vPoint2.y - vPoint1.y), // Find the interpolated Y
                           vPoint1.z + 0.01 * (vPoint2.z - vPoint1.z));// Find the interpolated Z // */
            }
        }

    // Stop rendering the triangles
    glEnd();

}
    unsigned char *pixels = NULL;

void Md2::createTexture( unsigned int textureArray[],char *strFileName,int textureID )
{
    SDL_Surface *pBitmap[1];
    if( strFileName == NULL )                           // Return from the function if no file name was passed in
        return ;
    
    // We need to load the texture data, so we use a cool function that SDL offers.

//    pBitmap[0] = SDL_LoadPCX(strFileName);              // Load the bitmap and store the data
        //pBitmap[0] = IMG_Load( strFileName );
		pBitmap[0] = SDL_LoadBMP(strFileName);              // Load the bitmap and store the data
			

    if(pBitmap[0] == NULL)                                // If we can't load the file, quit!
    {
        printf("Failed loading %s : %s\n", strFileName , SDL_GetError());
        Quit(0);
    }

    // Generate a texture with the associative texture ID stored in the array

    glGenTextures(1, &textureArray[textureID]);

    // Bind the texture to the texture arrays index and init the texture
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

    int width  = pBitmap[0] -> w;
    int height = pBitmap[0] -> h;
    unsigned char * data = (unsigned char *) (pBitmap[0] -> pixels);         // the pixel data

    int BytesPerPixel = pBitmap[0] -> format -> BytesPerPixel;

    for( int i = 0 ; i < (height / 2) ; ++i )
        for( int j = 0 ; j < width * BytesPerPixel; j += BytesPerPixel )
            for(int k = 0; k < BytesPerPixel; ++k)
                swap( data[ (i * width * BytesPerPixel) + j + k], data[ ( (height - i - 1) * width * BytesPerPixel ) + j + k]);

	    pixels = (unsigned char *)malloc(width * height * 3 );
		if( pixels == NULL )
		{
			printf("CANNOT MALLOC!!! - GOT NULL\n");
		}

    int count = 0;

    // the following lines extract R,G and B values from any bitmap

    for(int i = 0; i < (width * height); ++i)
    {
        byte r,g,b;                                                // R,G and B that we will put into pImage

        Uint32 pixel_value = 0;                                    // 32 bit unsigned int (as dictated by SDL)

        // the following loop extracts the pixel (however wide it is 8,16,24 or 32) and 
        // creates a long with all these bytes taken together.

        for(int j = BytesPerPixel - 1 ; j >=0; --j)                // for each byte in the pixel (from the right)
        {
            pixel_value = pixel_value << 8;                        // left shift pixel value by 8 bits
            pixel_value = pixel_value | data[ (i * BytesPerPixel) + j ];  // then make the last 8 bits of pixel value  =
        }                                                                 // the byte that we extract from pBitmap's data

        SDL_GetRGB(pixel_value, pBitmap[0] -> format, (Uint8 *)&r, (Uint8 *)&g, (Uint8 *)&b);     // here we get r,g,b from pixel_value which is stored in the form specified by pBitmap->format

        pixels[count++] = r;          // in our tImage classes we store r first
        pixels[count++] = g;          // then g
        pixels[count++] = b;          // and finally b (for bmps - three channels only)
        /*pixels[(i * BytesPerPixel) + 0] = r;          // in our tImage classes we store r first
        pixels[(i * BytesPerPixel) + 1] = g;          // then g
        pixels[(i * BytesPerPixel) + 2] = b;          // and finally b (for bmps - three channels only)*/

        pixel_value = 0;                                           // reset pixel , else we get incorrect values of r,g,b
    }

    // Build Mipmaps (builds different versions of the picture for distances - looks better)

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap[0]->w, pBitmap[0]->h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    // Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR is the smoothest.    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Now we need to free the bitmap data that we loaded since openGL stored it as a texture

    SDL_FreeSurface(pBitmap[0]);                        // Free the texture data we dont need it anymore

}

float Md2::ReturnCurrentTime( t3DModel *pModel, int nextFrame )
{
    float elapsedTime   = 0.0f;
//    float lastTime     = 0.0f;

    // Get the current time in milliseconds
    float time = SDL_GetTicks();

    // Find the time that has elapsed since the last time that was stored
    elapsedTime = time - lastTime;

    // To find the current t we divide the elapsed time by the ratio of 1 second / our anim speed.
    // Since we aren't using 1 second as our t = 1, we need to divide the speed by 1000
    // milliseconds to get our new ratio, which is a 5th of a second.
    float t = elapsedTime / (1000.0f / kAnimationSpeed);

    // If our elapsed time goes over a 5th of a second, we start over and go to the next key frame
    if (elapsedTime >= (1000.0f / kAnimationSpeed) )
    {
        // Set our current frame to the next key frame (which could be the start of the anim)
        pModel->currentFrame = nextFrame;

        // Set our last time to the current time just like we would when getting our FPS.
        lastTime = time;
    }

    // Return the time t so we can plug this into our interpolation.
    return t;

}

void Md2::setAnimation( Anim_Type t )
{
   switch( t )
   {
	   case STAND:
	       g_3DModel.currentAnim = AnimNum_Stand;
	      // Set the current frame to be the starting frame of the new animation
	       break;
	   case ATTACK:
	       g_3DModel.currentAnim = AnimNum_Attack;
	       break;
	   case DEATH:
	        g_3DModel.currentAnim = AnimNum_Death;
	        break;
	   case RUN:
	        g_3DModel.currentAnim = AnimNum_Run;
	        break;
	}
	g_3DModel.currentFrame = g_3DModel.pAnimations[g_3DModel.currentAnim].startFrame;
}

int Md2::getNumFrames( Anim_Type t )
{
		switch( t )
		{
			case STAND:
				return g_3DModel.pAnimations[AnimNum_Stand].endFrame - g_3DModel.pAnimations[AnimNum_Stand].startFrame;
			case ATTACK:
				return g_3DModel.pAnimations[AnimNum_Attack].endFrame - g_3DModel.pAnimations[AnimNum_Attack].startFrame;
			case DEATH:
				return g_3DModel.pAnimations[AnimNum_Death].endFrame - g_3DModel.pAnimations[AnimNum_Death].startFrame;
			case RUN:
				return g_3DModel.pAnimations[AnimNum_Run].endFrame - g_3DModel.pAnimations[AnimNum_Run].startFrame;
		}
}

Md2::~Md2( )
{
    for(int i = 0; i < g_3DModel.numOfObjects; i++)
    {
        // Free the faces, normals, vertices, and texture coordinates.
        delete [] g_3DModel.pObject[i].pFaces;
        delete [] g_3DModel.pObject[i].pNormals;
        delete [] g_3DModel.pObject[i].pVerts;
        delete [] g_3DModel.pObject[i].pTexVerts;
    }

}
