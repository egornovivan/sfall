
#include "Module.h"

class BarBoxes : public Module {
	const char* name() { return "BarBoxes"; }
	void init();
};

int _stdcall GetBox(int i);
void _stdcall AddBox(int i);
void _stdcall RemoveBox(int i);
