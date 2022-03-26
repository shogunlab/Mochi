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

#pragma once

#include <stdint.h>
#include <string>
#include "Windows.h"

#include <chaiscript/chaiscript.hpp>
#include "dyncall.h"

// Hacky fix for C2027 bug, undefined type ---------------
typedef struct DLLib_ DLLib;

struct DLLib_
{
	IMAGE_DOS_HEADER dos_header;
};

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DL_API
#define DL_API
#endif

	/* shared library loading and explicit symbol resolving */
	DL_API void* dlFindSymbol(DLLib* pLib, const char* pSymbolName);

#ifdef __cplusplus
}
#endif

// ---------------

chaiscript::ModulePtr registerFunCall();

class funCallObj
{
public:
	funCallObj();
	bool       init(DLLib* library, const std::string& symbolName, int callMode);

	void       evalVoid();
	//void*      evalPointer();
	float      evalFloat();
	double     evalDouble();
	int8_t     evalInt8();
	int16_t    evalInt16();
	int32_t    evalInt32();
	int64_t    evalInt64();
	uint8_t    evalUInt8();
	uint16_t   evalUInt16();
	uint32_t   evalUInt32();
	uint64_t   evalUInt64();

	//void       argPointer(void* arg);
	void       argFloat(float arg);
	void       argDouble(double arg);
	void       argInt8(int8_t arg);
	void       argInt16(int16_t arg);
	void       argInt32(int32_t arg);
	void       argInt64(int64_t arg);
	void       argUInt8(uint8_t arg);
	void       argUInt16(uint16_t arg);
	void       argUInt32(uint32_t arg);
	void       argUInt64(uint64_t arg);

private:
	DCCallVM* vm;
	DCpointer	func;
};
