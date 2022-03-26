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

#include "chaiHelpers.h"

DLLib* chai_LoadLib(const std::string& lib)
{
	return dlLoadLibrary(lib.c_str());
}

funCallObj* chai_InitFunCall(DLLib* libPtr, const std::string& function, int callMode)
{
	funCallObj* obj = NULL;

	if (libPtr) {

		funCallObj* obj = new funCallObj();
		if (obj) {

			if (false == obj->init(libPtr, function, callMode)) {

				std::cout << "Failed to load function " << function.c_str() << std::endl;
				delete obj;
				obj = NULL;
			}
		}
	}
	return obj;
}

void chai_printUint32Hex(uint32_t var)
{
	std::printf("%08x\r\n", var);
}

void chai_printUint64Hex(uint64_t var)
{
	std::printf("%016I64x\r\n", var);
}

void chai_dbgAddr(DLLib* library)
{
	std::printf("Library address: %p\r\n", library);
}

UINT_PTR chai_uintPtr(chaiscript::Boxed_Value& bv)
{
	if (bv.get_type_info().bare_equal_type_info(typeid(std::string)))
	{
		const std::string& str = chaiscript::boxed_cast<const std::string&>(bv);
		return (UINT_PTR)str.c_str();
	}
	else if (bv.get_type_info().bare_equal_type_info(typeid(std::vector<chaiscript::Boxed_Value>)))
	{

		auto vec = chaiscript::boxed_cast<std::vector<chaiscript::Boxed_Value>&>(bv);
		for (auto& i : vec)
		{
			auto a = i.get_const_ptr();

			if (i.get_type_info().bare_equal_type_info(typeid(char)))
			{
				std::printf("%p = %hhd\r\n", a, *(int8_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned char)))
			{
				std::printf("%p = %hhu\r\n", a, *(uint8_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(short)))
			{
				std::printf("%p = %hu\r\n", a, *(int16_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned short)))
			{
				std::printf("%p = %hu\r\n", a, *(uint16_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned int)) ||
				i.get_type_info().bare_equal_type_info(typeid(unsigned long)))
			{
				std::printf("%p = %u\r\n", a, *(uint32_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(int)) ||
				i.get_type_info().bare_equal_type_info(typeid(long)))
			{
				std::printf("%p = %d\r\n", a, *(int32_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(unsigned long long)) ||
				i.get_type_info().bare_equal_type_info(typeid(unsigned long int)))
			{
				std::printf("%p = %llu\r\n", a, *(uint64_t*)i.get_const_ptr());
			}
			else if (i.get_type_info().bare_equal_type_info(typeid(long long)) ||
				i.get_type_info().bare_equal_type_info(typeid(long int)))
			{
				std::printf("%p = %lld\r\n", a, *(int64_t*)i.get_const_ptr());
			}
		}

		return (UINT_PTR)0;
	}

	return chaiscript::boxed_cast<UINT_PTR>(bv);
}

void chai_pushArray(funCallObj* funCall, chaiscript::Boxed_Value& bv)
{
	if (bv.get_type_info().bare_equal_type_info(typeid(std::vector<chaiscript::Boxed_Value>)))
	{
		auto vec = chaiscript::boxed_cast<std::vector<chaiscript::Boxed_Value>&>(bv);
		for (auto& entry : vec)
		{
			if (entry.get_type_info() == typeid(char) ||
				entry.get_type_info() == typeid(unsigned char))
			{
				funCall->argUInt8(*(uint8_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(short) ||
				entry.get_type_info() == typeid(unsigned short))
			{
				funCall->argUInt16(*(uint16_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(long) ||
				entry.get_type_info() == typeid(int) ||
				entry.get_type_info() == typeid(unsigned long) ||
				entry.get_type_info() == typeid(unsigned int))
			{
				funCall->argUInt32(*(uint32_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(long long) ||
				entry.get_type_info() == typeid(long int) ||
				entry.get_type_info() == typeid(unsigned long long) ||
				entry.get_type_info() == typeid(unsigned long int))
			{
				funCall->argUInt64(*(uint64_t*)entry.get_const_ptr());
			}
			else if (entry.get_type_info() == typeid(std::string))
			{
				const std::string& str = chaiscript::boxed_cast<const std::string&>(bv);
				funCall->argUInt32((uint32_t)str.c_str());
			}
		}
	}
}
