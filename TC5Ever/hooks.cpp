#include "hooks.h"
#include <optional>
#include <detours.h>
#include <iostream>

namespace hooks {
	namespace wsock {

		static ConnectFuncPtr oConnect;
		static std::function<int(const ConnectFuncPtr orig, SOCKET s, const sockaddr* name, int namelen)> hConnect = nullptr;
		void HookConnect(std::function<int(const ConnectFuncPtr orig, SOCKET s, const sockaddr* name, int namelen)> cb) {
			hConnect = cb;
		}

		int WINAPI connectHook(SOCKET s, const sockaddr* name, int namelen) {
			static_assert(std::is_same<decltype(connectHook), ConnectFunc>::value, "ensure hook function type is the same");
			std::cout << "connect() hook called" << std::endl;
			if (hConnect) {
				return hConnect(oConnect, s, name, namelen);
			}
			return oConnect(s, name, namelen);
		};

	}

	void Attach() {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		wsock::oConnect = reinterpret_cast<wsock::ConnectFuncPtr>(DetourFindFunction("wsock32.dll", "connect"));
		DetourAttach(reinterpret_cast<PVOID*>(&wsock::oConnect), wsock::connectHook);
		DetourTransactionCommit();
		std::cout << "detours attached" << std::endl;
	};

	void Detach() {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(reinterpret_cast<PVOID*>(&wsock::oConnect), wsock::connectHook);
		DetourTransactionCommit();
		std::cout << "detours detached" << std::endl;
	};
}