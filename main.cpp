#include <iostream>
#include "Editor.hpp"
int main(int argc, const char ** argv)
{
	MSQE::Editor e;
	if (argc == 2){
		e.open(argv[1]);
	}
	e.userLoop();
}