#include "platform.h"

#include <stdio.h>

#ifndef WIN32
#include <dirent.h>
#endif

int get_list_of_files_in_dir(const char *dirname, int *num_files, char *dname[])
{
#ifdef  WIN32
   WIN32_FIND_DATA ffd;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError = 0;
   int i = 0;
   char buf[256];
   sprintf_s(buf, 256, "%s\\*", dirname);

   hFind = FindFirstFile(buf, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
       fprintf(stderr, "FindFirstFile failed: %d", dwError);
       return -1;
   } 

   do
   {
       if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
           continue;
      
       if (i >= *num_files) {
           fprintf(stderr, "Skipping files: %s as it exceeded dir limit: %d\n", ffd.cFileName, *num_files);
       } else {
          dname[i] = strdup(ffd.cFileName);
          i++;
       }
   } while (FindNextFile(hFind, &ffd) != 0);

   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
       fprintf(stderr, "Reading files failed: %d\n", dwError);
   }
   FindClose(hFind);

   *num_files = i;

#else   /* Linux */
	DIR *d = opendir("maps");
	struct dirent *dent;
	while ((dent = readdir(d))) {

    }
#endif
    return 0;
}

