/*
Original Source:

	https://github.com/domzigm/lcscript

License:

	Copyright (c) 2016, domzigm
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.

	* Neither the name of lcscript nor the names of its
	  contributors may be used to endorse or promote products derived from
	  this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define __CALLGATE__

#include "callGate.h"

std::vector<std::tuple<const type_info&, uint64_t>> stack;

uint32_t callFunc(chaiscript::ChaiScript& chai, const char* funcName, uint8_t dataLen, uint8_t* dataPtr)
{
	uint32_t retVal = 0;
	if (funcName == NULL || dataLen == NULL || dataPtr == NULL || *funcName == NULL) {
		return 0;
	}

	std::stringstream stream;

	stream << funcName << "(";
	for (auto& arg : stack)
	{
		std::string cast = "0";
		auto& type = std::get<0>(arg);
		auto& val = std::get<1>(arg);

		cast = std::to_string(static_cast<uint8_t>(val));

		if (type == typeid(uint8_t)) {
			cast = std::to_string((uint8_t)val);
		}
		else if (type == typeid(uint16_t)) {
			cast = std::to_string((uint16_t)val);
		}
		else if (type == typeid(uint32_t)) {
			cast = std::to_string((uint32_t)val);
		}
		else if (type == typeid(uint64_t)) {
			cast = std::to_string((uint64_t)val);
		}
		else if (type == typeid(int8_t)) {
			cast = std::to_string((int8_t)val);
		}
		else if (type == typeid(int16_t)) {
			cast = std::to_string((int16_t)val);
		}
		else if (type == typeid(int32_t)) {
			cast = std::to_string((int32_t)val);
		}
		else if (type == typeid(int64_t)) {
			cast = std::to_string((int64_t)val);
		}
		/*
		else if (type == typeid(float)) {
			cast = std::to_string((float)val);
		}
		else if (type == typeid(double)) {
			cast = std::to_string((double)val);
		}
		*/
		stream << cast << ",";
	}
	stream.seekp(-1, std::ios_base::end);
	stream << ");";

	try
	{
		auto ret = chai.eval(stream.str());
		memcpy(dataPtr, ret.get_const_ptr(), dataLen);
		retVal = 1;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (const std::string& e) {
		std::cout << e << std::endl;
	}
	catch (...) {
		std::cout << "Uncatchable exception occured" << std::endl;
	}

	stack.clear();

	return retVal;
}