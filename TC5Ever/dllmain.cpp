// dllmain.cpp : Defines the entry point for the DLL application
#include "framework.h"
#include "settings.h"
#include "hooks.h"
#include <string>
#include <ws2tcpip.h>
#include <iostream>

using namespace std::string_literals;
extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(HINSTANCE handle, DWORD version, REFIID r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk);

using DirectInput8CreateFunc = decltype(&DirectInput8Create);

static DirectInput8CreateFunc oDirectInput8Create;

HRESULT WINAPI DirectInput8Create(HINSTANCE handle, DWORD version, REFIID r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk) {
	return oDirectInput8Create(handle, version, r_iid, out_wrapper, p_unk);
}

bool restoreDirectInput8Create() {
	static_assert(sizeof(TCHAR) == sizeof(char), "only compile !utf16 mode");

	HMODULE hmod;
	char csyspath[320];
	GetSystemDirectory(csyspath, sizeof(csyspath));
	hmod = LoadLibrary((csyspath + "\\dinput8.dll"s).c_str());
	if (hmod == NULL) {
		return false;
	}
	oDirectInput8Create = reinterpret_cast<decltype(&DirectInput8Create)>(GetProcAddress(hmod, "DirectInput8Create"));
	return true;
}

void Main();

bool APIENTRY DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		if (!restoreDirectInput8Create()) {
			return false;
		}
		Main();
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
};

void CreateDebugConsole() {
	FILE *conin, *conout;
	AllocConsole();
	freopen_s(&conin, "conin$", "r", stdin);
	freopen_s(&conout, "conout$", "w", stdout);
	freopen_s(&conout, "conout$", "w", stderr);
}

void Main() {
	CreateDebugConsole();

	std::vector<std::string> args = settings::Args();

	std::string address = "75.52.93.251"s;
	std::string playerside = "1"s;

	for (std::string& arg : args) {
		static const std::string addrarg = "-address="s;
		static const std::string playerarg = "-playerside="s;

		size_t pos;
		if ((pos = arg.find(addrarg)) != std::string::npos) {
			std::string value = arg.substr(pos + addrarg.length());
			address = value;
		}
		if ((pos = arg.find(playerarg)) != std::string::npos) {
			std::string value = arg.substr(pos + playerarg.length());
		}
	}

	std::cout << "address: " << address << std::endl;
	std::cout << "playerside: " << playerside << std::endl;


	if (playerside == "2") {
		in_addr addr;
		inet_pton(AF_INET, address.c_str(), &addr);

		hooks::wsock::HookConnect([=](hooks::wsock::ConnectFuncPtr orig, SOCKET s, const sockaddr* n, int y) -> int {
			sockaddr_in* in = reinterpret_cast<sockaddr_in*>(const_cast<sockaddr*>(n));
			in->sin_addr = addr;
			return orig(s, n, y);
		});
	}

	hooks::Attach();
}
