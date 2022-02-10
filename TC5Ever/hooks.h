#pragma once
#include <WinSock2.h>
#include <functional>
#include <type_traits>

namespace hooks {
	namespace wsock {
		using ConnectFunc = decltype(connect);
		using ConnectFuncPtr = decltype(&connect);
		void HookConnect(std::function<int(const ConnectFuncPtr orig, SOCKET s, const sockaddr* name, int namelen)> hook);
	};

	void Attach();
	void Detach();
};