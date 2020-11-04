#include "Base/Application.h"

#if _DEBUG
int main() {
#else
#include <Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
	Application& app = Application::Instance();
	if (!app.Init()) {
		return -1;
	}
	app.Run();
	app.Shutdown();
	return 0;
}