#include "framework.h"
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <span>
#include <iterator>

namespace settings {
	std::vector<std::string> Args() {
		int nArgs;
		auto* wArgv = CommandLineToArgvW(GetCommandLineW(), &nArgs);

		std::span s(wArgv, nArgs);
		std::vector<std::string> args;

		std::transform(s.begin(), s.end(), std::back_inserter(args), [](auto wstr) -> std::string {
			auto wstrLen = wcslen(wstr);
			auto nChars = WideCharToMultiByte(CP_UTF8, 0, wstr, int(wstrLen), nullptr, 0, nullptr, nullptr);
			std::unique_ptr<char[]> buf = std::make_unique<char[]>(nChars);
			WideCharToMultiByte(CP_UTF8, 0, wstr, int(wstrLen), buf.get(), nChars, 0, 0);
			return buf.get();
		});

		return args;
	}
}
