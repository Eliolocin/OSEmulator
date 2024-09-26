#pragma once
#include "TypeDefs.h"
#include "Process.h"
#include "AbsConsole.h"

class MainConsole : public AbsConsole
{
public:
	MainConsole();
	void onEnabled() override;
	void display() override;
	void process() override;
};

