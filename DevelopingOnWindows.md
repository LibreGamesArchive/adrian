**If you are looking to just try the game on Windows, please visit the [Downloads](http://code.google.com/p/adrian/downloads/list) page.**


# Introduction #

Adrian uses Microsoft Visual C++ 2010 Express to build the software for Windows. You can download it [here](http://www.microsoft.com/visualstudio/eng/downloads#d-2010-express). We would be testing this on 32 bit windows.

# Details #

## Install GLEW ##

Use the [instructions](http://glew.sourceforge.net/install.html) from the GLEW home page and install 32 bit GLEW.

Version tested for: 1.9

## Install SDL ##

Download & Install SDL libraries from [here](http://www.libsdl.org/download-1.2.php).

Version tested for: 1.2.15

Also Install the following SDL sub-projects
  * [SDL\_image](http://www.libsdl.org/projects/SDL_image/) ([v1.2.12](http://www.libsdl.org/projects/SDL_image/release/SDL_image-devel-1.2.12-VC.zip))
  * [SDL\_ttf](http://www.libsdl.org/projects/SDL_ttf/) ([v2.0.11](http://www.libsdl.org/projects/SDL_ttf/release/SDL_ttf-devel-2.0.11-VC.zip))
  * [SDL\_mixer](http://www.libsdl.org/projects/SDL_mixer/) ([v1.2.12](http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-devel-1.2.12-VC.zip))


# Compatibility #

If you are doing windows development, you can use this page to build for windows. Please NOTE that the main developers develop on Linux, so try not to break compatibility - by using specific windows functionality. If you have to - then use the "#ifdef	WIN32" for such code - but it is discouraged in favor of portable solutions.