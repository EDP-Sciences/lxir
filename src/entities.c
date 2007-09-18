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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static int count = 0;
static const char RE[] = "\\[entity![^!]*!\\]";
regex_t re;


static void exit_regerror(int err) {
	size_t s = regerror(err, &re, 0, 0);
	char * buffer = malloc(s);
	regerror(err, &re, buffer, s);
	fprintf(stderr, "Regular expression error : %s\n", buffer);
	free(buffer);
	regfree(&re);
	exit(-1);
}

void start_entities() {
	if(count++ == 0) {
		int err = regcomp(&re, RE, 0);
		if (err) exit_regerror(err);
	}
}

void stop_entities() {
	if(--count == 0) {
		regfree(&re);
	}
}

char * strconcat(char * src, const char * dst, int len) {
	char * result;
	if(src) {
		int sl = strlen(src);
		result = malloc(sl + len + 1);
		memcpy(result, src, sl);
		memcpy(result + sl, dst, len);
		result[sl + len] = 0;
		free(src);
	} else {
		result = malloc(len + 1);
		memcpy(result, dst, len);
		result[len] = 0;
	}
	return result;
}

char * strentity(char * src, const char * dst, int len) {
	char * temp, * result;
	int offset = 8;
	if (dst[offset] == '\\') ++offset;
	
	temp = malloc(2 + len - (offset + 2));
	temp[0] = '&';
	memcpy(temp + 1, dst + offset, len - (offset + 2));
	temp[len - (offset + 1)] = ';';
	result = strconcat(src, temp, len - offset);
	free(temp);
	
	return result;
}

extern char * replace_entities(const char * src) {
	char * result = 0;
	int offset = 0;
	int len = strlen(src);
	regmatch_t match;
	
	if (count <= 0)
		return 0;
	while (1) {
		int err = regexec(&re, src + offset, 1, &match, 0);
		if (err == 0) {
			if (match.rm_so) {
				result = strconcat(result, src + offset, match.rm_so);
			}
			result = strentity(result, src + offset + match.rm_so, match.rm_eo - match.rm_so);
			offset += match.rm_eo;
		} else if (err == REG_NOMATCH) break;
		else exit_regerror(err);
	}
	if (result && offset < len) {
		result = strconcat(result, src + offset, len - offset);
	}
	return result;
}


#if TEST

int main() {
	start_entities();
	
	//~ replace_entities("[entity!copyright!]");
	printf(replace_entities("befor[!]e[entity!copyright!]between[entity!#123!]after\n"));
	printf("%s\n", replace_entities("d[entity!\\#x0323!]"));
	
	stop_entities();
	return 0;
}

#endif
