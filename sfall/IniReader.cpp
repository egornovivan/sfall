/*
 *    sfall
 *    Copyright (C) 2008-2021  The sfall team
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

#include "Utils.h"

#include "IniReader.h"

namespace sfall
{

DWORD modifiedIni;

static const char* ddrawIni = ".\\ddraw.ini";
static char ini[65] = ".\\";

static int __stdcall getInt(const char* section, const char* setting, int defaultValue, const char* iniFile) {
	return GetPrivateProfileIntA(section, setting, defaultValue, iniFile);
}

static size_t __stdcall getString(const char* section, const char* setting, const char* defaultValue, char* buf, size_t bufSize, const char* iniFile) {
	return GetPrivateProfileStringA(section, setting, defaultValue, buf, bufSize, iniFile);
}

static std::string __stdcall getString(const char* section, const char* setting, const char* defaultValue, size_t bufSize, const char* iniFile) {
	char* buf = new char[bufSize];
	getString(section, setting, defaultValue, buf, bufSize, iniFile);
	std::string str(buf);
	delete[] buf;
	return str;
}

static std::vector<std::string> __stdcall getList(const char* section, const char* setting, const char* defaultValue, size_t bufSize, char delimiter, const char* iniFile) {
	std::vector<std::string> list = split(getString(section, setting, defaultValue, bufSize, iniFile), delimiter);
	std::transform(list.cbegin(), list.cend(), list.begin(), (std::string (*)(const std::string&))trim);
	return list;
}

const char* GetConfigFile() {
	return ini;
}

void SetDefaultConfigFile() {
	std::strcpy(&ini[2], &ddrawIni[2]);
}

void SetConfigFile(const char* iniFile) {
	strcat_s(ini, iniFile);
}

int __stdcall GetIntDefaultConfig(const char* section, const char* setting, int defaultValue) {
	return getInt(section, setting, defaultValue, ddrawIni);
}

std::vector<std::string> __stdcall GetListDefaultConfig(const char* section, const char* setting, const char* defaultValue, size_t bufSize, char delimiter) {
	return getList(section, setting, defaultValue, bufSize, delimiter, ddrawIni);
}

int __stdcall GetConfigInt(const char* section, const char* setting, int defaultValue) {
	return getInt(section, setting, defaultValue, ini);
}

std::string __stdcall GetConfigString(const char* section, const char* setting, const char* defaultValue, size_t bufSize) {
	return trim(getString(section, setting, defaultValue, bufSize, ini));
}

size_t __stdcall GetConfigString(const char* section, const char* setting, const char* defaultValue, char* buf, size_t bufSize) {
	return getString(section, setting, defaultValue, buf, bufSize, ini);
}

std::vector<std::string> __stdcall GetConfigList(const char* section, const char* setting, const char* defaultValue, size_t bufSize) {
	return getList(section, setting, defaultValue, bufSize, ',', ini);
}

int __stdcall IniGetInt(const char* section, const char* setting, int defaultValue, const char* iniFile) {
	return getInt(section, setting, defaultValue, iniFile);
}

size_t __stdcall IniGetString(const char* section, const char* setting, const char* defaultValue, char* buf, size_t bufSize, const char* iniFile) {
	return getString(section, setting, defaultValue, buf, bufSize, iniFile);
}

std::string __stdcall IniGetString(const char* section, const char* setting, const char* defaultValue, size_t bufSize, const char* iniFile) {
	return getString(section, setting, defaultValue, bufSize, iniFile);
}

std::vector<std::string> __stdcall IniGetList(const char* section, const char* setting, const char* defaultValue, size_t bufSize, char delimiter, const char* iniFile) {
	return getList(section, setting, defaultValue, bufSize, delimiter, iniFile);
}

int __stdcall SetConfigInt(const char* section, const char* setting, int value) {
	char buf[33];
	_itoa_s(value, buf, 33, 10);
	int result = WritePrivateProfileStringA(section, setting, buf, ini);
	return result;
}

void IniReader_Init() {
	modifiedIni = GetConfigInt("Main", "ModifiedIni", 0);
}

}
