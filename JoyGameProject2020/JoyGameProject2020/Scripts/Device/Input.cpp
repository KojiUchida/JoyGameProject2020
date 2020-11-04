#include "Input.h"
#include "Math/Vector2.h"
#include <hidsdi.h>
#include <SetupAPI.h>
#include <vector>
#include <string>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")

static IDirectInput8* dinput;
static IDirectInputDevice8* devkeyboard;
static IDirectInputDevice8* devmouse;
static IDirectInputDevice8* devjoystick;

static BYTE previousKeyState[256];
static BYTE currentKeyState[256];
static DIMOUSESTATE currentMouseState;
static DIMOUSESTATE previousMouseState;
static DIJOYSTATE currentJoyState;
static DIJOYSTATE previousJoyState;
static HWND m_hwnd;
static const float stickDeadZone = ANALOG_STICK_VALUE / 100.0f * STICK_DEAD_ZONE_PARCENT;

JOYINFOEX JoyInfoEx;
HIDP_CAPS m_hidCaps;
HIDP_VALUE_CAPS m_hidValCaps;
HANDLE ds4Handle;

Input& Input::Instance() {
	static Input instance;
	return instance;
}

bool Input::Init(HWND hwnd, HINSTANCE hinstance) {
	m_hwnd = hwnd;

	auto result = DirectInput8Create(
		hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		return false;
	}

	/* キーボード */
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result)) {
		return false;
	}
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		return false;
	}
	result = devkeyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		return false;
	}

	/* マウス */
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	if (FAILED(result)) {
		return false;
	}
	result = devmouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) {
		return false;
	}
	result = devmouse->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}

	/* パッド */
	result = dinput->EnumDevices(
		DI8DEVCLASS_GAMECTRL, EnumJoystickCallBack, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(result)) {
		return false;
	}
	if (devjoystick == NULL) {
		return true;
	}
	result = devjoystick->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(result)) {
		return false;
	}
	result = devjoystick->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}
	result = devjoystick->EnumObjects(EnumAxisCallBack, hwnd, DIDFT_AXIS);
	if (FAILED(result)) {
		return false;
	}

	//InitController(hwnd);

	return true;
}

bool Input::InitController(HWND hwnd) {
	std::vector<std::wstring> paths;
	GUID guid;
	HidD_GetHidGuid(&guid);

	HDEVINFO deviceInfo;
	deviceInfo = SetupDiGetClassDevs(&guid, NULL, hwnd, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	//_ASSERT_EXPR(deviceInfo != INVALID_HANDLE_VALUE)

	SP_DEVICE_INTERFACE_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	for (DWORD index = 0;
		SetupDiEnumDeviceInterfaces(deviceInfo, 0, &guid, index, &deviceInfoData);
		++index) {
		DWORD requiredLength;
		// デバイスパスを含む構造体のサイズを取得
		SetupDiGetDeviceInterfaceDetail(deviceInfo, &deviceInfoData, NULL, 0, &requiredLength, NULL);
		// 取得したサイズを元にバッファを確保
		LPBYTE deviceDetailDataBuffer = new BYTE[requiredLength];
		PSP_DEVICE_INTERFACE_DETAIL_DATA deviceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)deviceDetailDataBuffer;
		deviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		// デバイスパスを取得
		SetupDiGetDeviceInterfaceDetail(deviceInfo, &deviceInfoData, deviceDetailData, requiredLength, &requiredLength, NULL);
		// デバイスパスをコピー
		paths.push_back(deviceDetailData->DevicePath);
		// バッファ削除
		delete[] deviceDetailDataBuffer;
	}
	// 使用したデバイス情報の破棄
	SetupDiDestroyDeviceInfoList(deviceInfo);

	for (auto i = 0; i < paths.size(); ++i) {
		/* デバイスへのハンドル作成 */
		HANDLE handle = ::CreateFile(paths[i].c_str(),
			(GENERIC_READ | GENERIC_WRITE), (FILE_SHARE_READ | FILE_SHARE_WRITE),
			NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

		/* 事前解析データの読み込み */
		PHIDP_PREPARSED_DATA preparsedData;
		if (HidD_GetPreparsedData(handle, &preparsedData)) {
			WCHAR deviceName[126];
			ULONG maxLength = sizeof(wchar_t) * 126;
			HidD_GetProductString(handle, deviceName, maxLength);

			/* コントローラーの列挙 */
			std::wstring deviceNameString = deviceName;
			if (deviceNameString.find(L"Controller") != std::wstring::npos) {
				ds4Handle = handle;

				//ReadFile(handle, )
				break;
			}

			HidD_FreePreparsedData(preparsedData);
		}

		/* ハンドルを閉じる */
		CloseHandle(handle);
	}

	return true;
}

void Input::Update() {
	for (int i = 0; i < _countof(currentKeyState); ++i) {
		previousKeyState[i] = currentKeyState[i];
	}
	previousMouseState = currentMouseState;
	previousJoyState = currentJoyState;

	HRESULT result;

	if (devkeyboard != NULL) {
		result = devkeyboard->Acquire();
		result = devkeyboard->GetDeviceState(sizeof(currentKeyState), currentKeyState);
	}

	if (devmouse != NULL) {
		result = devmouse->Acquire();
		result = devmouse->GetDeviceState(sizeof(DIMOUSESTATE), &currentMouseState);
	}

	if (devjoystick != NULL) {
		result = devjoystick->Poll();
		if (FAILED(result)) {
			result = devjoystick->Acquire();
			while (result == DIERR_INPUTLOST) {
				result = devjoystick->Acquire();
			}
		}
		result = devjoystick->GetDeviceState(sizeof(DIJOYSTATE), &currentJoyState);
	}
}

bool Input::IsKeyDown(BYTE key) {
	return previousKeyState[key] == OFF &&
		currentKeyState[key] == ON;
}

bool Input::IsKey(BYTE key) {
	return currentKeyState[key] == ON;
}

bool Input::IsKeyUp(BYTE key) {
	return previousKeyState[key] == ON &&
		currentKeyState[key] == OFF;
}

bool Input::IsMouseButtonDown(MouseButton button) {
	return previousMouseState.rgbButtons[(int)button] == OFF &&
		currentMouseState.rgbButtons[(int)button] == ON;
}

bool Input::IsMouseButton(MouseButton button) {
	return currentMouseState.rgbButtons[(int)button] == ON;
}

bool Input::IsMouseButtonUp(MouseButton button) {
	return previousMouseState.rgbButtons[(int)button] == ON &&
		currentMouseState.rgbButtons[(int)button] == OFF;
}

float Input::WheelValue() {
	return (float)currentMouseState.lZ;
}

Vector2 Input::MouseMove() {
	return Vector2((float)currentMouseState.lX, (float)currentMouseState.lY);
}

Vector2 Input::GetCursorPos() {
	POINT p{};
	::GetCursorPos(&p);
	ScreenToClient(m_hwnd, &p);
	return Vector2((float)p.x, (float)p.y);
}

void Input::SetCursorPos(const Vector2& pos) {
	POINT p{};
	ScreenToClient(m_hwnd, &p);
	::SetCursorPos((int)(-p.x + pos.x), (int)(-p.y + pos.y));
}

bool Input::IsButtonDown(PadButton button) {
	return previousJoyState.rgbButtons[(int)button] == OFF &&
		currentJoyState.rgbButtons[(int)button] == ON;
}

bool Input::IsButton(PadButton button) {
	return currentJoyState.rgbButtons[(int)button] == ON;
}

bool Input::IsButtonUp(PadButton button) {
	return previousJoyState.rgbButtons[(int)button] == ON &&
		currentJoyState.rgbButtons[(int)button] == OFF;
}

bool Input::IsDButtonDown(DPad button) {
	return previousJoyState.rgdwPOV[0] / 4500.0f != (float)button &&
		currentJoyState.rgdwPOV[0] / 4500.0f == (float)button;
}

bool Input::IsDButton(DPad button) {
	return currentJoyState.rgdwPOV[0] / 4500.0f == (float)button;
}

bool Input::IsDButtonUp(DPad button) {
	return previousJoyState.rgdwPOV[0] / 4500.0f == (float)button &&
		currentJoyState.rgdwPOV[0] / 4500.0f != (float)button;
}

Vector2 Input::LeftStickValue() {
	/* デッドゾーン設定のため物凄く長くなってしまった... */
	return Vector2(stickDeadZone < currentJoyState.lX ?
		(currentJoyState.lX - stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) :
		-stickDeadZone > currentJoyState.lX ?
		(currentJoyState.lX + stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) : 0,
		stickDeadZone < -currentJoyState.lY ?
		(-currentJoyState.lY - stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) :
		-stickDeadZone > -currentJoyState.lY ?
		(-currentJoyState.lY + stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) : 0
	);
}

Vector2 Input::RightStickValue() {
	/* デッドゾーン設定のため物凄く長くなってしまった... */
	return Vector2(stickDeadZone < currentJoyState.lZ ?
		(currentJoyState.lZ - stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) :
		-stickDeadZone > currentJoyState.lZ ?
		(currentJoyState.lZ + stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) : 0,
		stickDeadZone < -currentJoyState.lRz ?
		(-currentJoyState.lRz - stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) :
		-stickDeadZone > -currentJoyState.lRz ?
		(-currentJoyState.lRz + stickDeadZone) / (ANALOG_STICK_VALUE - stickDeadZone) : 0
	);
}

BOOL Input::EnumJoystickCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	auto result = dinput->CreateDevice(pdidInstance->guidInstance, &devjoystick, NULL);
	if (FAILED(result))return DIENUM_STOP;
	return DIENUM_CONTINUE;
}

BOOL Input::EnumAxisCallBack(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -ANALOG_STICK_VALUE;
	diprg.lMax = ANALOG_STICK_VALUE;

	auto result = devjoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result))return DIENUM_STOP;
	return DIENUM_CONTINUE;
}
