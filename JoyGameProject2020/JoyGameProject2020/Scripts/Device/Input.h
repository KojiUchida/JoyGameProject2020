#pragma once

#define DIRECTINPUT_VERSION 0x800
#define OFF 0x00
#define ON 0x80
#define STICK_DEAD_ZONE_PARCENT 10
#define ANALOG_STICK_VALUE 1000

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

enum class MouseButton {
	LEFT = 0,
	RIGHT = 1,
	CENTER = 2,
};

enum class PadButton {
	X = 0,
	Y = 1,
	A = 2,
	B = 3,
	L1 = 4,
	R1 = 5,
	L2 = 6,
	R2 = 7,
	LEFTSTICK = 8,
	RIGHTSTICK = 9,
	BACK = 10,
	START = 11,
	GUIDE = 12,
};

/* ゲームパッドの矢印キー */
enum class DPad {
	UP = 0,
	UPRIGHT = 1,
	RIGHT = 2,
	DOWNRIGHT = 3,
	DOWN = 4,
	DOWNLEFT = 5,
	LEFT = 6,
	UPLEFT = 7,
};

struct Vector2;
class Input {

private:
	Input() {}
	~Input() {}
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

public:
	static Input& Instance();
	bool Init(HWND hwnd, HINSTANCE hinstance);
	bool InitController(HWND hwnd);
	void Update();

	/* キーが押された瞬間か？ */
	static bool IsKeyDown(BYTE key);
	/* キーが押されているか？ */
	static bool IsKey(BYTE key);
	/* キーが離された瞬間か？ */
	static bool IsKeyUp(BYTE key);

	/* マウスが押された瞬間か？ */
	static bool IsMouseButtonDown(MouseButton button);
	/* マウスが押されているか？ */
	static bool IsMouseButton(MouseButton button);
	/* マウスが離された瞬間か？ */
	static bool IsMouseButtonUp(MouseButton button);
	/* マウスホイールの回転量の取得 */
	static float WheelValue();
	/* マウスの移動量の取得 */
	static Vector2 MouseMove();
	/* マウスカーソルの位置の取得 */
	static Vector2 GetCursorPos();
	/* マウスカーソルの位置の設定 */
	static void SetCursorPos(const Vector2& pos);

	/* ボタンが押された瞬間か？ */
	static bool IsButtonDown(PadButton button);
	/* ボタンが押されているか？ */
	static bool IsButton(PadButton button);
	/* ボタンが離された瞬間か？ */
	static bool IsButtonUp(PadButton button);

	/* ボタンが押された瞬間か？ */
	static bool IsDButtonDown(DPad button);
	/* ボタンが押されているか？ */
	static bool IsDButton(DPad button);
	/* ボタンが押された瞬間か？ */
	static bool IsDButtonUp(DPad button);

	/* 左スティックの傾き取得 */
	static Vector2 LeftStickValue();
	/* 右スティックの傾き取得 */
	static Vector2 RightStickValue();

private:
	/* ジョイスティック列挙用コールバック */
	static BOOL CALLBACK EnumJoystickCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	/* 軸列挙用コールバック */
	static BOOL CALLBACK EnumAxisCallBack(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
};

