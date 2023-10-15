#include "SolarisWM.h"
#include <x/x.h>
using namespace Ewok;

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	SolarisWM xwm;
	xwm.readConfig(x_get_theme_fname("/etc/x/themes", "", "xwm.conf"));	
	xwm.run();
	return 0;
}
