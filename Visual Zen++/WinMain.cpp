#include "pch.h"

// Global classes
MainDialogManager* MainDialog = nullptr;
PluginAPI::LoadedPlugin Plugin;

// Global structures
Communication::ConnectionInformation Connection;
CronusZen::CronusZenInformation Cronus;
Emulator::ConsoleInformation Console;
Emulator::DeviceInformation Device;

BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // TODO: Check if this fails (which it shouldn't)
    LoadLibraryA("RichEd20.dll");

    // Initialize main dialog
    MainDialog = new MainDialogManager(hInstance, DLG_MAIN);

    // Verify it was allocated and created
    if (MainDialog != nullptr) {

        // Make the dialog and display it
        MainDialog->MakeDialog();

        // Perform window message loop
        MSG Message;
        while (GetMessage(&Message, NULL, 0, 0)) {
            if (Message.message == WM_QUIT) {
                delete MainDialog;
                break;
            }
            else {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            }
        }
    }

	return 0;
}