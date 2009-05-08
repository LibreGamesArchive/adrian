/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2009  Bhanu Chetlapalli, Vamsi Krishna
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "platform.h"

#include <stdio.h>
#include <string.h>

#ifndef WIN32
#include <dirent.h>
#endif

int get_list_of_files_in_dir(const char *dirname, int *num_files, char *dname[])
{
   int i = 0;
#ifdef  WIN32
   WIN32_FIND_DATA ffd;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError = 0;
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

#else   /* Linux */
	DIR *d = opendir("maps");
	struct dirent *dent;
	while (((dent = readdir(d)) != NULL) && i < *num_files) {
		dname[i] = strdup(dent->d_name);
		i++;
    }
	closedir(d);
#endif
   *num_files = i;

    return 0;
}

