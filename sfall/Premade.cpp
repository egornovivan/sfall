/*
 *    sfall
 *    Copyright (C) 2009, 2010  The sfall team
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

#include "main.h"
#include "FalloutEngine.h"
#include "Message.h"

static PremadeChar* premade;

static const char* __fastcall GetLangPremadePath(const char* premadePath) {
	static char premadeLangPath[56]; // premade\<language>\combat.bio
	static bool isDefault = false;
	static long len = 0;

	if (isDefault) return nullptr;
	if (len == 0) {
		len = std::strlen(Message_GameLanguage());
		if (len == 0 || len >= 32) {
			isDefault = true;
			return nullptr;
		}
		isDefault = (_stricmp(Message_GameLanguage(), "english") == 0);
		if (isDefault) return nullptr;

		std::strncpy(premadeLangPath, premadePath, 8);
		std::strcpy(&premadeLangPath[8], Message_GameLanguage());
	}
	std::strcpy(&premadeLangPath[8 + len], &premadePath[7]);

	return premadeLangPath;
}

static const char* __fastcall PremadeGCD(const char* premadePath) {
	const char* path = GetLangPremadePath(premadePath);
	return (path && fo_db_access(path)) ? path : premadePath;
}

static DbFile* __fastcall PremadeBIO(const char* premadePath, const char* mode) {
	premadePath = GetLangPremadePath(premadePath);
	return (premadePath) ? fo_db_fopen(premadePath, mode) : nullptr;
}

static void __declspec(naked) select_display_bio_hook() {
	__asm {
		push eax;
		push edx;
		mov  ecx, eax; // premade path
		call PremadeBIO;
		test eax, eax;
		jz   default;
		add  esp, 8;
		retn;
default:
		pop  edx;
		pop  eax;
		jmp  db_fopen_;
	}
}

static void __declspec(naked) select_update_display_hook() {
	__asm {
		mov  ecx, eax; // premade path
		call PremadeGCD;
		jmp  proto_dude_init_;
	}
}

void Premade_Init() {
	std::vector<std::string> premadePaths = GetConfigList("misc", "PremadePaths", "", 512);
	std::vector<std::string> premadeFids = GetConfigList("misc", "PremadeFIDs", "", 512);
	if (!premadePaths.empty() && !premadeFids.empty()) {
		dlog("Applying premade characters patch.", DL_INIT);
		int count = min(premadePaths.size(), premadeFids.size());
		premade = new PremadeChar[count];
		for (int i = 0; i < count; i++) {
			std::string path = "premade\\" + premadePaths[i];
			if (path.size() > 19) {
				dlog_f(" Failed: %s exceeds 11 characters\n", DL_INIT, premadePaths[i].c_str());
				return;
			}
			std::strcpy(premade[i].path, path.c_str());
			premade[i].fid = atoi(premadeFids[i].c_str());
		}

		SafeWrite32(0x51C8D4, count);                  // _premade_total
		SafeWrite32(0x4A7D76, (DWORD)premade);         // select_update_display_
		SafeWrite32(0x4A8B1E, (DWORD)premade);         // select_display_bio_
		SafeWrite32(0x4A7E2C, (DWORD)&premade[0].fid); // select_display_portrait_
		std::strcpy((char*)0x50AF68, premade[0].path); // for selfrun
		dlogr(" Done", DL_INIT);
	}

	// Add language path for premade GCD/BIO files
	HookCall(0x4A8B44, select_display_bio_hook);
	HookCall(0x4A7D91, select_update_display_hook);
}
