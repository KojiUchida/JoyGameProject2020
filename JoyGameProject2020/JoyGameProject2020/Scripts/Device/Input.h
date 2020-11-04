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

/* �Q�[���p�b�h�̖��L�[ */
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

	/* �L�[�������ꂽ�u�Ԃ��H */
	static bool IsKeyDown(BYTE key);
	/* �L�[��������Ă��邩�H */
	static bool IsKey(BYTE key);
	/* �L�[�������ꂽ�u�Ԃ��H */
	static bool IsKeyUp(BYTE key);

	/* �}�E�X�������ꂽ�u�Ԃ��H */
	static bool IsMouseButtonDown(MouseButton button);
	/* �}�E�X��������Ă��邩�H */
	static bool IsMouseButton(MouseButton button);
	/* �}�E�X�������ꂽ�u�Ԃ��H */
	static bool IsMouseButtonUp(MouseButton button);
	/* �}�E�X�z�C�[���̉�]�ʂ̎擾 */
	static float WheelValue();
	/* �}�E�X�̈ړ��ʂ̎擾 */
	static Vector2 MouseMove();
	/* �}�E�X�J�[�\���̈ʒu�̎擾 */
	static Vector2 GetCursorPos();
	/* �}�E�X�J�[�\���̈ʒu�̐ݒ� */
	static void SetCursorPos(const Vector2& pos);

	/* �{�^���������ꂽ�u�Ԃ��H */
	static bool IsButtonDown(PadButton button);
	/* �{�^����������Ă��邩�H */
	static bool IsButton(PadButton button);
	/* �{�^���������ꂽ�u�Ԃ��H */
	static bool IsButtonUp(PadButton button);

	/* �{�^���������ꂽ�u�Ԃ��H */
	static bool IsDButtonDown(DPad button);
	/* �{�^����������Ă��邩�H */
	static bool IsDButton(DPad button);
	/* �{�^���������ꂽ�u�Ԃ��H */
	static bool IsDButtonUp(DPad button);

	/* ���X�e�B�b�N�̌X���擾 */
	static Vector2 LeftStickValue();
	/* �E�X�e�B�b�N�̌X���擾 */
	static Vector2 RightStickValue();

private:
	/* �W���C�X�e�B�b�N�񋓗p�R�[���o�b�N */
	static BOOL CALLBACK EnumJoystickCallBack(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	/* ���񋓗p�R�[���o�b�N */
	static BOOL CALLBACK EnumAxisCallBack(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
};

