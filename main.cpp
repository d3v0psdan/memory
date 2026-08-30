// memory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include "memory.h"

int main()
{
	std::unique_ptr<memory::Memory> memory = std::make_unique<memory::Memory>();
	
	bool attached = memory->attachToProcess("explorer.exe");
	
	attached 
		? std::cout << "Successfully attached to process." << std::endl 
		: std::cout << "Failed to attach to process." << std::endl;
	
	std::cin.get();
	return 0;
}
