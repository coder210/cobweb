#include "cobweb.h"


int 
main(int argc, char** argv) {
	if (argc <= 1) {
		ccp_error("Need a config file. Please read cobweb config filename");
		return 0;
	}

	const char* config_file = argv[1];
	int ret = cobweb_run(config_file);

	/* app exit */
	ccp_green_print("bye");
	return ret;
}
