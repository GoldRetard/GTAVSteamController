// ======================================================================
// STEAM CONTROLLER NATIVE FOR GRAND THEFT AUTO V
// Copyright (C) 2016 Ilya Egorov (goldrenard@gmail.com)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// ======================================================================

#include "stdafx.h"

#ifdef SCRIPT_ASI
#include "HookedScript.h"
#else
#include "InjectedScript.h"
MODULEINFO g_MainModuleInfo = { 0 };
BOOL g_CompatibleVersion;
#endif

BaseScript *m_BaseScript = NULL;

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            Logger::Init(hInstance);
            DEBUGOUT(L"Steam Controller Native initialized.");

            m_BaseScript =
#ifdef SCRIPT_ASI
                HookedScript::GetInstance();
#else
                InjectedScript::GetInstance();

            g_CompatibleVersion = VersionUtils::IsValidEnvironment();
            if (g_CompatibleVersion) {
                if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &g_MainModuleInfo, sizeof(g_MainModuleInfo))) {
                    Logger::Fatal(L"Unable to get MODULEINFO from GTA5.exe");
                }
                DEBUGOUT(L"GTA5 [0x%I64X][0x%X]", g_MainModuleInfo.lpBaseOfDll, g_MainModuleInfo.SizeOfImage);
            }
            else {
                DEBUGOUT(L"Incompatible game version found. Native call will not be invoked.");
            }
#endif
            m_BaseScript->Start(hInstance);
            break;
        case DLL_PROCESS_DETACH:
            if (m_BaseScript != NULL) {
                m_BaseScript->Shutdown();
            }
            break;
    }
    return TRUE;
}