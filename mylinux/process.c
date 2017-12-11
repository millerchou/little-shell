#include<stdio.h>
#include"ltlsh.h"
int is_control_command(char*);//是shell控制命令吗	
int do_control_command(char**);//执行shell控制命令
int ok_to_execute();
int process(char ** args) {
	int rv = 0;
	if (args[0] == NULL)
		rv = 0;
	else if (is_control_command(args[0]))
		rv = do_control_command(args);
	else if (ok_to_execute())
		rv = execute(args);
	return rv;
}