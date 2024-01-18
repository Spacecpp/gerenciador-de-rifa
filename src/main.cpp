#include "init.h"
#include "ProgramManager.h"


int main(int argc, char **argv)
{
	
	if (!init_program() ) return -1;
	
	{
		ProgramManager program;
		program.main_loop();
	}
	
	shutdown();
	
    return 0;
	
}
