#include <Windows.h>

#include <WinApp.h>
#include <DXCommon.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	OutputDebugStringA("Hello\n");

	WinApp* winApp = WinApp::GetInstance();
	DXCommon* dxCommon = DXCommon::GetInstance();




	winApp->Initialize();

	dxCommon->Initialize();




	while (!winApp->ProcessMessage()) {





		dxCommon->PreDraw();


		dxCommon->PostDraw();

	}



	dxCommon->Finalize();
	winApp->Finalize();

	return 0;
}