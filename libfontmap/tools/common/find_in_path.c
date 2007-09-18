/*
lxir - a tool to convert latex documents to xml.
Copyright (C) 2007  EDP-Sciences

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int find_in_path(const char * path, const char * extension, int (*handler)(const char *)) {
	DIR * dir;
	struct dirent * entry;
	struct stat fstat;
	int err = 0;

	dir = opendir(path);
	if(!dir) return -1;
	
	while(!err && (entry = readdir(dir))) {
		char * ext, * fname;
		if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) continue;
		fname = malloc(strlen(path) + strlen(entry->d_name) + 2);
		sprintf(fname, "%s/%s", path, entry->d_name);
		stat(fname, &fstat);
		if (S_ISDIR(fstat.st_mode)) {
			find_in_path(fname, extension, handler);
		}
		ext = strrchr(fname, '.');
		if(ext && strcmp(ext, extension) == 0) {
			err = handler(fname);
		}
		free(fname);
	}
	
	closedir(dir);
	return err;
}
