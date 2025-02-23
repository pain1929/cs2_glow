#include <Windows.h>
#include <cstdint>
#include "output/offsets.hpp"
#include "output/client_dll.hpp"




extern void glow(uint64_t);

// 工作线程
DWORD mainThread(void* arg) {


	auto client_base = (uint64_t)GetModuleHandle("client.dll");
	auto entity_list_ptr = (client_base + cs2_dumper::offsets::client_dll::dwEntityList);

	auto entity_list = *((uint64_t*)entity_list_ptr);

	auto entry = *(uint64_t*)(entity_list + 0x10);


	while (true) {

		auto local = *(uint64_t*)(client_base + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);


		auto teamNum = *(uint8_t*)(local + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

		for (int i = 0; i < 64; i++) {

			auto controller = *(uint64_t*)(entry + i * 0x78);

			if (!controller) {
				continue;
			}

			auto playerPawnHandle = *(uint64_t*)(controller + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);

			if (!playerPawnHandle) {
				continue;
			}

			auto entry_2 = *(uint64_t*)(entity_list + 0x8 * ((playerPawnHandle & 0x7fff) >> 9) + 0x10);

			auto current = *(uint64_t*)((entry_2 + 0x78 * (playerPawnHandle & 0x1ff)));

			auto currentTeamNum = *(uint8_t*)(current + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

			if (currentTeamNum != teamNum) {
				glow(current);
			}

			

		}
	}

	return 0;
	
	
	
}


static void glow(uint64_t current) {
	const auto glowColorOverride = current + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_Glow + cs2_dumper::schemas::client_dll::CGlowProperty::m_glowColorOverride;
	const auto glowFunc = current + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_Glow + cs2_dumper::schemas::client_dll::CGlowProperty::m_bGlowing;

	*(uint64_t*)(glowColorOverride) = 0x800000FF;

	*(bool*)glowFunc = true;
}



BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved) {

	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0,mainThread, NULL, 0, NULL);
	}

	return 0;

}