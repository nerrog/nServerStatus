// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。

//nServerStatus Beta 0.5.0

#include "pch.h"
#include "framework.h"
#include <iostream>
#include "GetMemory.h"
#include <timeapi.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

static bool useOP = true;
std::chrono::system_clock::time_point  start, end;

//TPS
static int CurrentTPS;
static int TPScount;
std::chrono::system_clock::time_point  now, prew;

void entry();
void loadCfg();


extern "C" {
    _declspec(dllexport) void onPostInit() {
        std::ios::sync_with_stdio(false);
        loadCfg();
        entry();
    }
}

//コンフィグロード
void loadCfg() {
    try {
        ConfigJReader jr("plugins\\nServerStatus\\nss.json");
        jr.bind("useOP", useOP, true);
    }
    catch (string e) {
        std::cout << "[nServerStatus]json Load error! Use default settings\n";
        
    }
}

//TPS計算

THook(void, "?tick@Level@@UEAAXXZ", void* self) {
    original(self);
    TPScount++;
    now = std::chrono::system_clock::now();
    int interval = std::chrono::duration_cast<std::chrono::milliseconds>(now - prew).count();
    
    if (interval > 1000) {
        if (TPScount > 20) {
            TPScount = 20;
        }

        CurrentTPS = TPScount;

        TPScount = 0;

        prew = std::chrono::system_clock::now();
    }

}


    bool oncmd_status(CommandOrigin const& ori, CommandOutput & outp){
    //get Uptime
        end = std::chrono::system_clock::now();
        int elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        //float elapsed_h = elapsed_sec / 3600;
    //Get RAM
        //int persentram = getPersentRAM();
        //int tortalram = getTotalRAM();

    outp.addMessage("---Server Status---\n"
    "Uptime: "+ std::to_string(elapsed_sec)+" Sec\n"
    "TPS: "+ std::to_string(CurrentTPS)+"\n"
    //"RamUsed (Total Ram)"+ std::to_string(persentram) +"("+ std::to_string(tortalram) +")"
    );
    return true;
}

    bool oncmd_tps(CommandOrigin const& ori, CommandOutput& outp) {
        outp.addMessage(std::to_string(CurrentTPS));
        return true;
    }

void entry(){
    //コマンド登録
    Event::addEventListener([](RegCmdEV ev) {
        CMDREG::SetCommandRegistry(ev.CMDRg);
        int permission_level;
        if (useOP) {
            permission_level = 1;
        }
        else
        {
            permission_level = 0;
        }

        MakeCommand("status", "Show Server Status", permission_level);
        MakeCommand("tps", "Show tps", 1);
        CmdOverload(status, oncmd_status);
        CmdOverload(tps, oncmd_tps);
        });

    //プラグイン実行時の時間を取得
    start = std::chrono::system_clock::now();
    prew = std::chrono::system_clock::now();

    CurrentTPS = 0;

    std::cout << "[nServerStatus]Loaded.\n";
}
