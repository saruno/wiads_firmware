/*
	http-client-c 
	Copyright (C) 2012-2013  Swen Kooij
	
	This file is part of http-client-c.

    http-client-c is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    http-client-c is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with http-client-c. If not, see <http://www.gnu.org/licenses/>.

	Warning:
	This library does not tend to work that stable nor does it fully implent the
	standards described by IETF. For more information on the precise implentation of the
	Hyper Text Transfer Protocol:
	
	http://www.ietf.org/rfc/rfc2616.txt
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
	#include <locale>
#endif

const int _DEBUG_ = 0;

/*
	Gets the offset of one string in another string
*/
int str_index_of(const char *a, char *b)
{
	char *offset = (char*)strstr(a, b);
	return offset - a;
}

/*
	Checks if one string contains another string
*/
int str_contains(const char *haystack, const char *needle)
{
	char *pos = (char*)strstr(haystack, needle);
	if(pos)
		return 1;
	else
		return 0;
}

/*
	Removes last character from string
*/
char* trim_end(char *string, char to_trim)
{
	char last_char = string[strlen(string) -1];
	if(last_char == to_trim)
	{
		char *new_string = string;
		new_string[strlen(string) - 1] = 0;
		return new_string;
	}
	else
	{
		return string;
	}
}

/*
	Concecates two strings, a wrapper for strcat from string.h, handles the resizing and copying
*/
char* str_cat(char *a, char *b)
{
	char *target = (char*)malloc(strlen(a) + strlen(b) + 1);
	strcpy(target, a);
	strcat(target, b);
	return target;
}

/* 
	Converts an integer value to its hex character
*/
char to_hex(char code) 
{
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

/*
	Replacement for the string.h strndup, fixes a bug
*/
char *str_ndup (const char *str, size_t max)
{
    size_t len = strnlen (str, max);
    char *res = (char*)malloc (len + 1);
    if (res)
    {
        memcpy (res, str, len);
        res[len] = '\0';
    }
    return res;
}

/*
	Replacement for the string.h strdup, fixes a bug
*/
char *str_dup(const char *src)
{
   char *tmp = (char*)malloc(strlen(src) + 1);
   if(tmp)
       strcpy(tmp, src);
   return tmp;
}

/*
 	Search and replace a string with another string , in a string
*/
char *str_replace(char *pattern , char *replacement , char *original)
{
    size_t const replen = strlen(replacement);
    size_t const patlen = strlen(pattern);
    size_t const orilen = strlen(original);

    size_t patcnt = 0;
    const char * oriptr;
    const char * patloc;

    // find how many times the pattern occurs in the original string
    for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
    {
        patcnt++;
    }

    {
        // allocate memory for the new string
        size_t const retlen = orilen + patcnt * (replen - patlen);
        char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

        if (returned != NULL)
        {
            // copy the original string,
            // replacing all the instances of the pattern
            char * retptr = returned;
            for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
            {
                size_t const skplen = patloc - oriptr;
                // copy the section until the occurence of the pattern
                strncpy(retptr, oriptr, skplen);
                retptr += skplen;
                // copy the replacement
                strncpy(retptr, replacement, replen);
                retptr += replen;
            }
            // copy the rest of the string.
            strcpy(retptr, oriptr);
        }
        return returned;
    }
    return NULL;
}

/*
	Get's all characters until '*until' has been found
*/
char* get_until(char *haystack, char *until)
{
	int offset = str_index_of(haystack, until);
	return str_ndup(haystack, offset);
}
