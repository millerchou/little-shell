#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ltlsh.h"
char * next_cmd(char * prompt, FILE * fp) { //输入保存为堆上字符串
	char * buf;
	int bufallsize = 0;
	int pos = 0;
	int c;

	printf("%s", prompt);
	while ((c = getc(fp)) != EOF) {
		if (pos + 1 >= bufallsize) {		
			if (bufallsize == 0)				
				buf = emalloc(BUFSIZ);	
			else							
				buf = erealloc(buf, bufallsize + BUFSIZ);
			bufallsize += BUFSIZ;
		}
		if (c == '\n')
			break;
		buf[pos++] = c;
	}
	if (c == EOF && pos == 0) {	
		free(buf);
		return NULL;
	}
	buf[pos] = '\0';
	return buf;
}

#define is_space(x) ((x)==' '||(x)=='\t')

char ** splitline(char * line) {
	char * newstr();
	char **args;			//存储指针
	int spots = 0;
	int bufallsize = 0;
	int argnum = 0;
	char * tmp = line;
	char * start;
	int len;

	if (line == NULL)
		return NULL;

	args = emalloc(BUFSIZ);		//分配指针空间
	bufallsize = BUFSIZ;
	spots = BUFSIZ / sizeof(char*);		//可存放指针数

	while (*tmp != '\0') {
		while (is_space(*tmp))
			tmp++;
		if (*tmp == '\0')
			break;

		if (argnum + 1 >= spots) {
			args = erealloc(args, bufallsize + BUFSIZ);	
			bufallsize += BUFSIZ;
			spots += (BUFSIZ / sizeof(char*));
		}

		start = tmp;
		len = 1;
		while (*++tmp != '\0' && !(is_space(*tmp)))
			len++;
		args[argnum++] = newstr(start, len);
	}
	args[argnum] = NULL;
	return args;
}
char * newstr(char * s, int l) {
	char * rv = emalloc(l + 1);
	rv[l] = '\0';
	strncpy(rv, s, l);
	return rv;
}
void freelist(char**list) {
	char **cp = list;
	while (*cp)
		free(*cp++);
	free(list);
}
void * emalloc(size_t n) {
	void * rv;
	if ((rv = malloc(n)) == NULL)
		fatal("out of memory","", 1);
	return rv;
}
void * erealloc(void * p, size_t n) {
	void * rv;
	if ((rv = realloc(p, n)) == NULL)
		fatal("realloc() failed", "", 1);
	return rv;
}
