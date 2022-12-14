/*
デバッグクラス
*/
#pragma once

#include "../Main/main.h"
#include "psapi.h"

class Debug
{

public:

	// 実行時間表示(コンソール)
	template <typename T>
	static void OutputRuntime(std::string title, T end, T start) {

		const std::chrono::duration<double> elapsed = end - start;

		std::string str = title + " Cost : " + std::to_string(elapsed.count()) + " sec";

		DebugOutputString(str.c_str());
	}

	// 使用メモリ表示(コンソール)(MB)
	static void OutputMemoryUsage() {
		HANDLE hProc = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS_EX pmc;
		BOOL isSuccess = GetProcessMemoryInfo(
			hProc,
			(PROCESS_MEMORY_COUNTERS*)&pmc,
			sizeof(pmc));
		CloseHandle(hProc);
		std::string str = "Memory Usage " + std::to_string(pmc.PrivateUsage >> 10) + " KB";
		DebugOutputString(str.c_str());
	}

	// 使用メモリGetter(KB)
	static float GetMemoryUsage() {

		HANDLE hProc = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS_EX pmc;
		BOOL isSuccess = GetProcessMemoryInfo(
			hProc,
			(PROCESS_MEMORY_COUNTERS*)&pmc,
			sizeof(pmc));
		CloseHandle(hProc);

		return (float)(pmc.PrivateUsage >> 10);
	}

};

