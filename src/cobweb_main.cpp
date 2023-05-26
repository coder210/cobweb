#include "cobweb.h"
#include "platform.h"



int
main(int argc, char** argv) {
	if (argc <= 1) {
		PlatformSystem::error("Need a config file. Please read CobwebSystem config filename");
		return 0;
	}

	const char* config_file = argv[1];
	int ret = CobwebSystem::Start(config_file);
	PlatformSystem::info("bye");

	return ret;
}
