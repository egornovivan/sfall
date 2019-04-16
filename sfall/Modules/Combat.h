/*
 *    sfall
 *    Copyright (C) 2008, 2009, 2011  The sfall team
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Module.h"

namespace sfall
{

struct KnockbackModifier {
	long id;
	DWORD type;
	double value;
};

class Combat : public Module {
public:
	const char* name() { return "Combat"; }
	void init();

	static std::vector<KnockbackModifier> mWeapons;
};

struct ChanceModifier {
	long id;
	int maximum;
	int mod;

	ChanceModifier() : id(0), maximum(95), mod(0) {}

	ChanceModifier(long _id, int max, int _mod) {
		id = _id;
		maximum = max;
		mod = _mod;
	}

	void SetDefault() {
		maximum = 95;
		mod = 0;
	}
};

void _stdcall SetHitChanceMax(fo::GameObject* critter, DWORD maximum, DWORD mod);
void _stdcall KnockbackSetMod(fo::GameObject* object, DWORD type, float val, DWORD on);
void _stdcall KnockbackRemoveMod(fo::GameObject* object, DWORD on);

void _stdcall SetNoBurstMode(fo::GameObject* critter, bool on);
void _stdcall DisableAimedShots(DWORD pid);
void _stdcall ForceAimedShots(DWORD pid);

}
