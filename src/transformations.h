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
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H 1

/* #define USE_DOUBLE_LINKED_LIST 1 */

struct xmlTransformationStack;

typedef struct xmlTransformationEntry {
	const char * parameter;
	struct xmlTransformationStack * stack;
} xmlTransformationEntry;

typedef void (*xmlTransformation)(xmlNodePtr, xmlTransformationEntry *);

void xmlTransformationSetReport(int);

void xmlTransformationRegister(const char *, const char *, xmlTransformation, const char *);
void xmlTransformationPush(xmlNodePtr, xmlTransformation, xmlTransformationEntry *);
int xmlTransformationAddToList(const char *, const char *, const char *);
int xmlTransformationApplyList(const char *, xmlDocPtr *);

void xmlTransformationInit();
void xmlRegisterTextTransformations();
void xmlRegisterMathTransformations();

#endif
