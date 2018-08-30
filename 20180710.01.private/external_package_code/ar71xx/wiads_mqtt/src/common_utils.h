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

/*
struct splits_array * str_splits(char * str, char * delim) {
    struct splits_array * ap = (struct splits_array*)malloc(sizeof(struct splits_array));
    ap->arr = NULL;
    ap->arr_len = 0;

    char * p = strtok (str, delim);
    while (p) {
        ap->arr = realloc (ap->arr, sizeof (char*) * ++ap->arr_len);
        if (ap->arr == NULL)
            return NULL;
        ap->arr[ap->arr_len-1] = p;
        p = strtok (NULL, delim);
    }
    ap->arr = realloc (ap->arr, sizeof (char*) * (ap->arr_len+1));
    ap->arr[ap->arr_len] = 0;
    return ap;
}
*/

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

void file_replace_str(char * confFileName, char *text_to_find , char *text_to_replace) {
    FILE *input = fopen(confFileName, "r");
    FILE *output = fopen("temp.txt", "w");
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        //172.16.116.157 localhost
        char *pos = strstr(buffer, text_to_find);
        if (pos != NULL)
        {
            /* Allocate memory for temporary buffer */
            char *temp = calloc(
                                strlen(buffer) - strlen(text_to_find) + strlen(text_to_replace) + 1, 1);
            
            /* Copy the text before the text to replace */
            //config_ip
            memcpy(temp, buffer, pos - buffer);
            /* Copy in the replacement text */
            //config_ip 127.0.0.1
            memcpy(temp + (pos - buffer), text_to_replace, strlen(text_to_replace));
            /* Copy the remaining text from after the replace text */
            //config_ip 127.0.0.1 localhost
            memcpy(temp + (pos - buffer) + strlen(text_to_replace),
                   pos + strlen(text_to_find),
                   1 + strlen(buffer) - ((pos - buffer) + strlen(text_to_find)));
            fputs(temp, output);
            free(temp);
        }
        else
            fputs(buffer, output);
    }
    
    fclose(output);
    fclose(input);    
    /* Rename the temporary file to the original file */
    rename("temp.txt", confFileName);
}

void file_change_key_value(char * confFileName, char *key , char *key_value) {
    FILE *input = fopen(confFileName, "r");
    FILE *output = fopen("temp.txt", "w");
    char buffer[1024];
    char *breakline = "\n";
    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        char *pos = strstr(buffer, key);
        if (pos != NULL)
        {
            /* Allocate memory for temporary buffer */
            char *temp = calloc(
                                strlen(buffer) - strlen(pos) + strlen(key_value) + strlen(breakline)
                                + 1, 1);
            memcpy(temp, buffer, pos - buffer);
            memcpy(temp + (pos - buffer), key_value, strlen(key_value));
            memcpy(temp + (pos - buffer) + strlen(key_value), breakline, strlen(breakline));
            fputs(temp, output);
            free(temp);
        } else {
            fputs(buffer, output);
        }
    }
    fclose(output);
    fclose(input);
    /* Rename the temporary file to the original file */
    int ret = rename("temp.txt", confFileName);
//    if(ret == 0) {
//          printf("File renamed successfully\n");
//    } else {
//          printf("Error: unable to rename the file\n");
//    }
}

void file_change_key_value_after_segment(char * confFileName, char *key , char *key_value, char *segment_content) {
    FILE *input = fopen(confFileName, "r");
    FILE *output = fopen("temp.txt", "w");
    char buffer[1024];
    int is_segment = 0;
    char *breakline = "\n";
    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        char *pos_segment = strstr(buffer, segment_content);
        if (pos_segment != NULL) {
            is_segment = 1;
        }
        if (is_segment) {
            char *pos = strstr(buffer, key);
            if (pos != NULL)
            {
                /* Allocate memory for temporary buffer */
                char *temp = calloc(
                                    strlen(buffer) - strlen(pos) + strlen(key_value) + strlen(breakline)
                                    + 1, 1);
                memcpy(temp, buffer, pos - buffer);
                memcpy(temp + (pos - buffer), key_value, strlen(key_value));
                memcpy(temp + (pos - buffer) + strlen(key_value), breakline, strlen(breakline));
                is_segment = 0;
                fputs(temp, output);
                free(temp);
            } else {
                fputs(buffer, output);
            }
        } else {
            fputs(buffer, output);
        }
    }
    fclose(output);
    fclose(input);
    /* Rename the temporary file to the original file */
    rename("temp.txt", confFileName);
}

void file_append_str(char * confFileName, char * str) {
    FILE *out = fopen(confFileName, "a");
    fprintf(out, "%s", str);
    fclose(out);
}

//Key la host name chu ko phai IP
void file_change_hosts(char * confFileName, char *key, char *key_value) {
    FILE *input = fopen(confFileName, "r");
    FILE *output = fopen("temp.txt", "w");
    char buffer[1024];
    char *breakline = "\n";
    int host_exist = 0;
    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        char *pos = strstr(buffer, key);
        if (pos != NULL)
        {
            host_exist = 1;
            /* Allocate memory for temporary buffer */
            char *temp = calloc(strlen(key_value) + strlen(breakline) + 1, 1);
            memcpy(temp, key_value, strlen(key_value));
            memcpy(temp + strlen(key_value), breakline, strlen(breakline));
            fputs(temp, output);
            free(temp);
        } else {
            fputs(buffer, output);
        }
    }
    fclose(output);
    fclose(input);
    /* Rename the temporary file to the original file */
    rename("temp.txt", confFileName);
    if (!host_exist) {
        file_append_str(confFileName, key_value);
    }
}

char * file_read_first_line(char * confFileName) {
    FILE *input = fopen(confFileName, "r");
    char buffer[1024];
    char * content = NULL;
    while(fgets(buffer, sizeof(buffer), input) != NULL) {
    	if (buffer != NULL) {
    		content = strdup(buffer);
    		break;
    	}
    }
    fclose(input);
    trim(content);
    return content;
}

char * file_read_line_contain_key(char * confFileName, char * key) {
    FILE *input = fopen(confFileName, "r");
    char buffer[1024];
    char * content = NULL;
    while (fgets(buffer, sizeof(buffer), input) != NULL)
    {
        char *pos = strstr(buffer, key);
        if (pos != NULL)
        {
            content = strdup(buffer);
            trim(content);
            break;
        }
    }
    fclose(input);
    return content;
}

char * get_string_startwith_in_array(char ** arr, int arr_len, char * prefix) {
    char * result = NULL;
    for (int i = 0; i < arr_len; i++) {
        trim(arr[i]);
        if (str_start_with(arr[i], prefix)) {
            result = arr[i];
            break;
        }
    }
    return result;
}

char * get_string_contain_a_not_b(char ** arr, int arr_len, char * a, char * b) {
    char * result = NULL;
    for (int i = 0; i < arr_len; i++) {
    	char * dup = strdup(arr[i]);
        if (strstr(dup, b) == NULL && strstr(dup, a) != NULL) {
            result = arr[i];
            free(dup);
            break;
        } else {
        	free(dup);
        }
    }
    return result;
}

void http_params_append(char * params, char * key, char * value) {
//	printf("Http append %s, value %s \n", key, value);
    strcat(params, key);
    strcat(params, "=");
    strcat(params, value);
    strcat(params, "&");
}

#endif /* common_utils_h */
