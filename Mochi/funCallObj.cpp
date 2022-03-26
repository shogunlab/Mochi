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

#include "funCallObj.h"

chaiscript::ModulePtr funCallMod;

chaiscript::ModulePtr registerFunCall()
{
	funCallMod = chaiscript::ModulePtr(new chaiscript::Module());

	chaiscript::utility::add_class<funCallObj>(
		*funCallMod,
		"funCall",
		{ /* Constructor */
			chaiscript::constructor<funCallObj()>(),
			chaiscript::constructor<funCallObj(const funCallObj&)>()
		},
		{ /* Members */
	#define FUN(x) { chaiscript::fun(&funCallObj::##x), #x }
			FUN(init),
			FUN(argDouble),
			FUN(argFloat),
			FUN(argInt16),
			FUN(argInt32),
			FUN(argInt64),
			FUN(argInt8),
			/*FUN(argPointer),*/
			FUN(argUInt16),
			FUN(argUInt32),
			FUN(argUInt64),
			FUN(argUInt8),
			FUN(evalDouble),
			FUN(evalFloat),
			FUN(evalInt16),
			FUN(evalInt32),
			FUN(evalInt64),
			FUN(evalInt8),
			//FUN(evalPointer),
			FUN(evalUInt16),
			FUN(evalUInt32),
			FUN(evalUInt64),
			FUN(evalUInt8),
			FUN(evalVoid)
		}

		);

	return funCallMod;
}

funCallObj::funCallObj()
{
}

bool funCallObj::init(DLLib* library, const std::string& symbolName, int callMode)
{
	bool bRet = false;

	if (0 != library) {

		vm = dcNewCallVM(64);
		if (vm) {

			dcMode(vm, (DCint)callMode);
			dcReset(vm);
			func = dlFindSymbol(library, symbolName.c_str());
			if (0 == func) {
				dcFree(vm);
			}
			else {
				bRet = true;
			}
		}
	}

	return bRet;
}

void funCallObj::evalVoid()
{
	dcCallVoid(vm, func);
	dcReset(vm);
}

//void* funCallObj::evalPointer()
//{
//	auto ret = (void*)dcCallPointer(vm, func);
//	dcReset(vm);
//	return ret;
//}

float funCallObj::evalFloat()
{
	auto ret = (float)dcCallFloat(vm, func);
	dcReset(vm);
	return ret;
}

double funCallObj::evalDouble()
{
	auto ret = (double)dcCallDouble(vm, func);
	dcReset(vm);
	return ret;
}

int8_t funCallObj::evalInt8()
{
	auto ret = (int8_t)dcCallChar(vm, func);
	dcReset(vm);
	return ret;
}

int16_t funCallObj::evalInt16()
{
	auto ret = (int16_t)dcCallShort(vm, func);
	dcReset(vm);
	return ret;
}

int32_t funCallObj::evalInt32()
{
	auto ret = (int32_t)dcCallLong(vm, func);
	dcReset(vm);
	return ret;
}

int64_t funCallObj::evalInt64()
{
	auto ret = (int64_t)dcCallLongLong(vm, func);
	dcReset(vm);
	return ret;
}

uint8_t funCallObj::evalUInt8()
{
	auto ret = (uint8_t)dcCallChar(vm, func);
	dcReset(vm);
	return ret;
}

uint16_t funCallObj::evalUInt16()
{
	auto ret = (uint16_t)dcCallShort(vm, func);
	dcReset(vm);
	return ret;
}

uint32_t funCallObj::evalUInt32()
{
	auto ret = (uint32_t)dcCallLong(vm, func);
	dcReset(vm);
	return ret;
}

uint64_t funCallObj::evalUInt64()
{
	auto ret = (uint64_t)dcCallLongLong(vm, func);
	dcReset(vm);
	return ret;
}

//void funCallObj::argPointer(void* arg)
//{
//	dcArgPointer(vm, (DCpointer)arg);
//}

void funCallObj::argFloat(float arg)
{
	dcArgFloat(vm, (DCfloat)arg);
}

void funCallObj::argDouble(double arg)
{
	dcArgDouble(vm, (DCdouble)arg);
}

void funCallObj::argInt8(int8_t arg)
{
	dcArgChar(vm, (DCchar)arg);
}

void funCallObj::argInt16(int16_t arg)
{
	dcArgShort(vm, (DCshort)arg);
}

void funCallObj::argInt32(int32_t arg)
{
	dcArgLong(vm, (DClong)arg);
}

void funCallObj::argInt64(int64_t arg)
{
	dcArgLongLong(vm, (DClonglong)arg);
}

void funCallObj::argUInt8(uint8_t arg)
{
	dcArgChar(vm, (DCchar)arg);
}

void funCallObj::argUInt16(uint16_t arg)
{
	dcArgShort(vm, (DCshort)arg);
}

void funCallObj::argUInt32(uint32_t arg)
{
	dcArgLong(vm, (DClong)arg);
}

void funCallObj::argUInt64(uint64_t arg)
{
	dcArgLongLong(vm, (DClonglong)arg);
}
