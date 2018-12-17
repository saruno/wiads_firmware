//
//  common_utils.h
//  TestC
//
//  Created by Mac One on 6/21/18.
//  Copyright © 2018 Mac One. All rights reserved.
//

#ifndef common_utils_h
#define common_utils_h

struct splits_array {
    char ** arr;
    int arr_len;
};

void splits_array_free(struct splits_array * options) {
	if (options != NULL) {
		free(options);
	}
}

struct splits_array * str_splits(char * str, char * delimiter) {
    struct splits_array * ap = (struct splits_array*)malloc(sizeof(struct splits_array));
    ap->arr = NULL;
    ap->arr_len = 0;

    char *text, *p, *first, **array;
    int c;
    char** ret;

    text=strdup(str);//strdup not standard
    if(text==NULL) return ap;
    for(c=0,p=text;NULL!=(p=strtok(p, delimiter));p=NULL, c++)//count item
        if(c==0) first=p; //first token top

    ret=(char**)malloc(sizeof(char*)*c+1);//+1 for NULL
    if(ret==NULL){
        free(text);
        return ap;
    }
    strcpy(text, str+(first-text));//skip until top token
    array=ret;
    for(p=text;NULL!=(p=strtok(p, delimiter));p=NULL){
        *array++=p;
    }
    *array=NULL;

    ap->arr = ret;
    ap->arr_len = c;
    return ap;
}

int str_start_with(char *str, const char *pre)
{
//    if(strncmp(str, pre, strlen(pre)) == 0) return 1;
//    return 0;
	    char cp;
	    char cs;

	    if (!*pre)
	        return 0;

	    if (str == NULL)
	    	return 0;

	    while ((cp = *pre++) && (cs = *str++))
	    {
	        if (cp != cs)
	            return 0;
	    }

	    if (!cs)
	        return 0;

	    return 1;
}

int str_equal(char * input,char *  check)
{
    int i,result=1;
    if (input == NULL || check == NULL) {
        return 0;
    }
    for(i=0; input[i]!='\0' || check[i]!='\0'; i++) {
        if(input[i] != check[i]) {
            result=0;
            break;
        }
    }
    return result;
}

/*C substring function: It returns a pointer to the substring */

char *str_substring(char *string, int position, int length)
{
   char *pointer;
   int c;

   pointer = (char*)malloc(length+1);

   if (pointer == NULL)
   {
//      printf("Unable to allocate memory.\n");
      exit(1);
   }

   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *(string+position-1);
      string++;
   }

   *(pointer+c) = '\0';

   return pointer;
}

void str_toupper(char * s) {
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }
}

void trim(char *str)
{
    char *start, *end;
    /* Find first non-whitespace */
    for (start = str; *start; start++)
    {
        if (!isspace((unsigned char)start[0]))
            break;
    }
    /* Find start of last all-whitespace */
    for (end = start + strlen(start); end > start + 1; end--)
    {
        if (!isspace((unsigned char)end[-1]))
            break;
    }
    *end = 0; /* Truncate last whitespace */
    
    /* Shift from "start" to the beginning of the string */
    if (start > str)
        memmove(str, start, (end - start) + 1);
}

#endif /* common_utils_h */
