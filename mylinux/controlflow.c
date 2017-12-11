#include<stdio.h>
#include"ltlsh.h"

enum states { NEUTRAL, WANT_THEN, THEN_BLOCK };//wantthen������execute
enum results{SUCCESS,FAIL};
static int state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(char*);
int ok_to_execute() {
	int rv = 1;
	if (state == WANT_THEN) {
		syn_err("then expected");
		rv = 0;
	}
	else if (state == THEN_BLOCK&&if_result == FAIL)
		rv = 0;
	return rv;
}
int is_control_command(char * s) {
	return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 ||
		strcmp(s, "fi") == 0);
}
int do_control_command(char ** args) {
	char * cmd = args[0];
	int rv = -1;

	if (strcmp(cmd, "if") == 0) {		//����if����
		if (state != NEUTRAL)		//��ʱ����neutral
			rv = syn_err("if unexpected");		//��������
		else
		{
			last_stat = process((args + 1)); //ִ��if�������
			if_result = (last_stat == 0 ? SUCCESS : FAIL);
			state = WANT_THEN;			//if�������������then��ʼǰ������
			rv = 0;
		}
	}
	else if (strcmp(cmd, "then") == 0) {	//����then����
		if (state != WANT_THEN)				//��ʱ����then֮ǰ��wantthen����
			rv = syn_err("then unexpected"); //��������
		else
		{
			state = THEN_BLOCK;
			rv = 0;
		}
	}
	else if (strcmp(cmd, "fi") == 0) {	//if������
		if (state != THEN_BLOCK)		//��ʱ����fi֮ǰ��then����
			rv = syn_err("fi unexpected");//��������
		else
		{
			state = NEUTRAL;
			rv = 0;
		}
	}
	else
		fatal("internal erroe processing:", cmd, 2);
	return rv;
}

int syn_err(char * msg)
{
	state = NEUTRAL;
	fprintf(stderr, "syntax error:%s\n", msg);
	return -1;
}
