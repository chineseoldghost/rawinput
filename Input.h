#pragma once

#include "RawInputAPI.h"
#include "RawDevice.h"

#include <exception>
#include <map>
#include <string>
#include <vector>

namespace RawInput
{
	class INPUT_API Input {
		Input(const Input &);
		Input & operator=(const Input &);
	public:
		explicit Input(const HWND = 0);
		~Input(void) throw();

		/*
		 * Basic Update() method. Call it on WM_INPUT messages
		 *
		 * @param1: LPARAM from main window's WndProc()
		 */
		LRESULT Update(HWND &, UINT &, WPARAM &, LPARAM &);

		/*
		 * Buffered Update() method. Call it on WM_INPUT messages
		 *
		 * @param1: LPARAM from main window's WndProc()
		 */
		//LRESULT UpdateBuff(const LPARAM & lParam); // Bugged?

		/*
		 * Call Change() to handle WM_INPUT_DEVICE_CHANGE messages
		 */
		LRESULT Change(WPARAM &, LPARAM &);

		/*
		 * Clean()s the state for all devices. Needs to be called at the end of each loop.
		 */
		void Clean(void);

		/*
		 * KeyUp() returns true on key release.
		 *
		 * @param1: one of Keyboard::keyboard_vkeys enum or VK code.
		 */
		bool KeyUp(const unsigned short &);

		/*
		 * KeyDown() returns true on key press.
		 *
		 * @param1: one of Keyboard::keyboard_vkeys enum or VK code.
		 */
		bool KeyDown(const unsigned short &);

		/*
		 * KeyHeld() returns true while key is pressed.
		 *
		 * @param1: one of Keyboard::keyboard_vkeys enum or VK code.
		 */
		bool KeyHeld(const unsigned short &);

		/*
		 * MouseButtonHeld() returns true if button is being pressed.
		 *
		 * @param1: one of Mouse::mouse_rawstate enum.
		 */
		bool MouseButtonHeld(const unsigned long &);

		/*
		 * MouseEvent() returns true on button event.
		 *
		 * @param1: one of Mouse::mouse_flags enum.
		 */
		bool MouseEvent(const unsigned short &);

		/*
		 * MousePos() returns mouse's x/y position.
		 *
		 * @param1: returns mouse last position X.
		 * @param2: returns mouse last position Y.
		 */
		void MousePos(long &, long &);
	private:
		/*
		 * GetSystemDevices() returns a vector with all the devices
		 * connected to the system at the time of call.
		 */
		std::vector<RAWINPUTDEVICELIST> GetSystemDevices(void) const;

		RID_DEVICE_INFO GetDeviceInfo(const HANDLE &);

		std::wstring GetDeviceName(const HANDLE &);

		//http://www.microsoft.com/whdc/archive/HID_HWID.mspx
		const static unsigned short HID_USAGE_PAGE				= 0x01;

		const static unsigned short HID_DEVICE_SYSTEM_MOUSE		= 0x02;
		const static unsigned short HID_DEVICE_SYSTEM_KEYBOARD	= 0x06;
		const static unsigned short HID_DEVICE_SYSTEM_GAME		= 0x04;
		//const static unsigned short HID_DEVICE_SYSTEM_CONTROL	= 0x80;

		std::vector<RAWINPUTDEVICE> ri_registered_devices_;

		std::map<HANDLE, Device<RAWMOUSE> > ri_mice_;
		std::map<HANDLE, Device<RAWKEYBOARD> > ri_keybs_;
		std::map<HANDLE, Device<RAWHID> > ri_hids_;

		Device<RAWMOUSE> ri_mouse_;
		Device<RAWKEYBOARD> ri_keyboard_;
		Device<RAWHID> ri_hid_;
	};

	struct Input_Exception : public std::exception {
		const std::wstring error_msg;
		const HRESULT hresult;

		Input_Exception(const std::wstring & c = 0, HRESULT hr = 0) : error_msg(c), hresult(hr) {};
	};
}