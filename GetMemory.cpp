#include "windows.h"
#include <string>
#include "pch.h"
//�������擾�p



int getPersentRAM() {
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	return (int)physMemUsed;
}

int getTotalRAM() {
	 MEMORYSTATUSEX memInfo;
	 memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	 GlobalMemoryStatusEx(&memInfo);
	 DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	 return (int)totalPhysMem;
}