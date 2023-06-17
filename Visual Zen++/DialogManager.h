#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H
#pragma once

typedef INT_PTR(CALLBACK* DialogWindowProc)(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam);

namespace DialogManager
{

	class DialogHandler {
	public:

		// Create dialog window:
		inline HWND MakeDialog() {
			return CreateDialogParam(Instance, MAKEINTRESOURCE(ResourceID), 0, (DLGPROC)MessageRouter, (LPARAM)this);
		}

		// User variables:
		HWND Handle;
		HINSTANCE Instance;

	private:

		// Message router:
		static inline INT_PTR CALLBACK MessageRouter(HWND WindowHandle, UINT WindowMessage, WPARAM wParam, LPARAM lParam) {

			DialogHandler* Dialog = nullptr;

			if (WindowMessage == WM_INITDIALOG) {
				Dialog = (DialogHandler*)lParam;

				if (Dialog != nullptr) {
					Dialog->Handle = WindowHandle;
					SetWindowLongPtr(WindowHandle, GWLP_USERDATA, (LPARAM)Dialog);
				}
			}
			else {
				Dialog = (DialogHandler*)GetWindowLongPtr(WindowHandle, GWLP_USERDATA);
			}

			return (Dialog != nullptr) ? Dialog->HandleMessage(WindowMessage, wParam, lParam) : FALSE;
		}

	protected:
		// Message handler:
		virtual INT_PTR CALLBACK HandleMessage(UINT WindowMessage, WPARAM wParam, LPARAM lParam) = 0;
		
		// Dialog information
		int ResourceID;
	};

	// User functions:
	static HWND DisplayDialog(HINSTANCE Instance, int DialogID, DialogWindowProc WindowProc) {
		return CreateDialogParam(Instance, MAKEINTRESOURCE(DialogID), NULL, WindowProc, 0);
	}


}

#endif
