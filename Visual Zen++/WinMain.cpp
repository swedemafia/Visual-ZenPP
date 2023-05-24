#include "pch.h"

// Global classes
MainDialogManager* MainDialog = nullptr;

// Global structures
Communication::ConnectionInformation Connection;
CronusZen::CronusZenInformation Cronus;
Emulator::ConsoleInformation Console;
Emulator::DeviceInformation Device;

BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LoadLibraryA("RichEd20.dll");

    MainDialog = new MainDialogManager(hInstance, DLG_MAIN);

    if (MainDialog != nullptr) {

        MainDialog->MakeDialog();

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