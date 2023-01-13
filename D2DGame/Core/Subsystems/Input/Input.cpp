#include "stdafx.h"
#include "Input.h"
#include "Input_Implementation.h"

namespace Input_Data
{
    XINPUT_STATE gamepad;
    uint gamepad_num = 0;
}

Input::Input(Context * const context)
    : ISubsystem(context)
{ 
}

bool Input::Initialize()
{
    const auto window_handle = Settings::Get().GetWindowhandle();
    if (!window_handle)
    {
        LOG_ERROR("Invalid parameter");
        return false;
    }

    //Register mouse
    {
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC  ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif

        RAWINPUTDEVICE rid[1];
        rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
        rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
        rid[0].dwFlags = RIDEV_INPUTSINK;
        rid[0].hwndTarget = window_handle;
        RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE));
    }
        
    return true;
}

void Input::Update(const float & delta_time)
{
	OnWindowData();

    if (is_check_for_new_device)
    {
        ZeroMemory(&Input_Data::gamepad, sizeof(XINPUT_STATE));
        is_gamepad_connected = XInputGetState(Input_Data::gamepad_num, &Input_Data::gamepad) == ERROR_SUCCESS;

        if (is_gamepad_connected)
        {
            LOG_INFO("Gamepad connected");
        }
        else
        {
            LOG_INFO("Gamepad disconnected");
        }

        is_check_for_new_device = false;
    }

    if (is_gamepad_connected)
    {
        keys[start_index_gamepad] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
        keys[start_index_gamepad + 1] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
        keys[start_index_gamepad + 2] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
        keys[start_index_gamepad + 3] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
        keys[start_index_gamepad + 4] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A;
        keys[start_index_gamepad + 5] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_B;
        keys[start_index_gamepad + 6] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_X;
        keys[start_index_gamepad + 7] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
        keys[start_index_gamepad + 6] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_START;
        keys[start_index_gamepad + 7] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
        keys[start_index_gamepad + 8] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
        keys[start_index_gamepad + 9] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
        keys[start_index_gamepad + 10] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        keys[start_index_gamepad + 11] = Input_Data::gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

        gamepad_trigger_left = static_cast<float>(Input_Data::gamepad.Gamepad.bLeftTrigger);
        gamepad_trigger_right = static_cast<float>(Input_Data::gamepad.Gamepad.bRightTrigger);
        gamepad_thumb_left.x = static_cast<float>(Input_Data::gamepad.Gamepad.sThumbLX);
        gamepad_thumb_left.y = static_cast<float>(Input_Data::gamepad.Gamepad.sThumbLY);
        gamepad_thumb_right.x = static_cast<float>(Input_Data::gamepad.Gamepad.sThumbRX);
        gamepad_thumb_right.y = static_cast<float>(Input_Data::gamepad.Gamepad.sThumbRY);

        if (gamepad_trigger_left != 0)	gamepad_trigger_left /= 255.0f;	// Convert [0, 255] to [0, 1]
        if (gamepad_trigger_right != 0)	gamepad_trigger_right /= 255.0f;	// Convert [0, 255] to [0, 1]
        if (gamepad_thumb_left.x != 0)	gamepad_thumb_left.x = gamepad_thumb_left.x < 0 ? gamepad_thumb_left.x / 32768.0f : gamepad_thumb_left.x / 32767.0f; // Convert [-32768, 32767] to [-1, 1]
        if (gamepad_thumb_left.y != 0)	gamepad_thumb_left.y = gamepad_thumb_left.y < 0 ? gamepad_thumb_left.y / 32768.0f : gamepad_thumb_left.y / 32767.0f; // Convert [-32768, 32767] to [-1, 1]
        if (gamepad_thumb_right.x != 0)	gamepad_thumb_right.x = gamepad_thumb_right.x < 0 ? gamepad_thumb_right.x / 32768.0f : gamepad_thumb_right.x / 32767.0f; // Convert [-32768, 32767] to [-1, 1]
        if (gamepad_thumb_right.y != 0)	gamepad_thumb_right.y = gamepad_thumb_right.y < 0 ? gamepad_thumb_right.y / 32768.0f : gamepad_thumb_right.y / 32767.0f; // Convert [-32768, 32767] to [-1, 1]
    }
    else
    {
        for (auto i = start_index_gamepad; i <= start_index_gamepad + 11; i++)
            keys[i] = false;
    }

    is_new_frame = true;
}

void Input::OnWindowData()
{
#define is_pressed(key_code) (::GetKeyState(key_code) & 0x8000) != 0
    if (is_new_frame)
    {
        mouse_delta = D3DXVECTOR2{};
        keys_previous_frame = keys;
        is_check_for_new_device = false;
    }

    const WindowData& window_data = context->GetEngine()->GetWindowData();

    //Mouse
    {
        //Keys
        //keys[start_index_mouse + 0] = (::GetKeyState(VK_LBUTTON) & 0x8000) != 0;
		keys[start_index_mouse + 0] = is_pressed(VK_LBUTTON);
        keys[start_index_mouse + 1] = (::GetKeyState(VK_MBUTTON) & 0x8000) != 0;
        keys[start_index_mouse + 2] = (::GetKeyState(VK_RBUTTON) & 0x8000) != 0;

        //Delta
        if (window_data.message == WM_INPUT)
        {
            uint dwSize = 48;
            static BYTE lpb[48];

            GetRawInputData((HRAWINPUT)window_data.lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
            RAWINPUT* raw = (RAWINPUT*)lpb;
            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                mouse_delta.x = static_cast<float>(raw->data.mouse.lLastX);
                mouse_delta.y = static_cast<float>(raw->data.mouse.lLastY);
            }
        }

        //Position
        if (window_data.handle == ::GetActiveWindow())
        {
            POINT mouse_screen_position;
            if (::GetCursorPos(&mouse_screen_position))
            {
                ::ScreenToClient(window_data.handle, &mouse_screen_position);
				mouse_position = D3DXVECTOR2(static_cast<float>(mouse_screen_position.x) - window_data.width*0.5f, window_data.height*0.5f - static_cast<float>(mouse_screen_position.y));

            }
        }

        //Wheel
        int wheel_delta = ::GetScrollPos(window_data.handle, SB_VERT);
        mouse_wheel_delta = static_cast<float>(wheel_delta - mouse_wheel);
        mouse_wheel = wheel_delta;
    }

    //Keyboard
    {

        // FUNCTION
        keys[0] = is_pressed(VK_F1);
        keys[1] = is_pressed(VK_F2);
        keys[2] = is_pressed(VK_F3);
        keys[3] = is_pressed(VK_F4);
        keys[4] = is_pressed(VK_F5);
        keys[5] = is_pressed(VK_F6);
        keys[6] = is_pressed(VK_F7);
        keys[7] = is_pressed(VK_F8);
        keys[8] = is_pressed(VK_F9);
        keys[9] = is_pressed(VK_F10);
        keys[10] = is_pressed(VK_F11);
        keys[11] = is_pressed(VK_F12);
        keys[12] = is_pressed(VK_F13);
        keys[13] = is_pressed(VK_F14);
        keys[14] = is_pressed(VK_F15);
        // NUMBERS
        keys[15] = is_pressed('0');
        keys[16] = is_pressed('1');
        keys[17] = is_pressed('2');
        keys[18] = is_pressed('3');
        keys[19] = is_pressed('4');
        keys[20] = is_pressed('5');
        keys[21] = is_pressed('6');
        keys[22] = is_pressed('7');
        keys[23] = is_pressed('8');
        keys[24] = is_pressed('9');
        // KEYPAD
        keys[25] = is_pressed(VK_NUMPAD0);
        keys[26] = is_pressed(VK_NUMPAD1);
        keys[27] = is_pressed(VK_NUMPAD2);
        keys[28] = is_pressed(VK_NUMPAD3);
        keys[29] = is_pressed(VK_NUMPAD4);
        keys[30] = is_pressed(VK_NUMPAD5);
        keys[31] = is_pressed(VK_NUMPAD6);
        keys[32] = is_pressed(VK_NUMPAD7);
        keys[33] = is_pressed(VK_NUMPAD8);
        keys[34] = is_pressed(VK_NUMPAD9);
        // LETTERS
        keys[35] = is_pressed('Q');
        keys[36] = is_pressed('W');
        keys[37] = is_pressed('E');
        keys[38] = is_pressed('R');
        keys[39] = is_pressed('T');
        keys[40] = is_pressed('Y');
        keys[41] = is_pressed('U');
        keys[42] = is_pressed('I');
        keys[43] = is_pressed('O');
        keys[44] = is_pressed('P');
        keys[45] = is_pressed('A');
        keys[46] = is_pressed('S');
        keys[47] = is_pressed('D');
        keys[48] = is_pressed('F');
        keys[49] = is_pressed('G');
        keys[50] = is_pressed('H');
        keys[51] = is_pressed('J');
        keys[52] = is_pressed('K');
        keys[53] = is_pressed('L');
        keys[54] = is_pressed('Z');
        keys[55] = is_pressed('X');
        keys[56] = is_pressed('C');
        keys[57] = is_pressed('V');
        keys[58] = is_pressed('B');
        keys[59] = is_pressed('N');
        keys[60] = is_pressed('M');
        // CONTROLS
        keys[61] = is_pressed(VK_ESCAPE);
        keys[62] = is_pressed(VK_TAB);
        keys[63] = is_pressed(VK_LSHIFT);
        keys[64] = is_pressed(VK_RSHIFT);
        keys[65] = is_pressed(VK_LCONTROL);
        keys[66] = is_pressed(VK_RCONTROL);
        keys[67] = is_pressed(VK_LMENU);
        keys[68] = is_pressed(VK_RMENU);
        keys[69] = is_pressed(VK_SPACE);
        keys[70] = is_pressed(VK_CAPITAL);
        keys[71] = is_pressed(VK_BACK);
        keys[72] = is_pressed(VK_RETURN);
        keys[73] = is_pressed(VK_DELETE);
        keys[74] = is_pressed(VK_LEFT);
        keys[75] = is_pressed(VK_RIGHT);
        keys[76] = is_pressed(VK_UP);
        keys[77] = is_pressed(VK_DOWN);
        keys[78] = is_pressed(VK_PRIOR);
        keys[79] = is_pressed(VK_NEXT);
        keys[80] = is_pressed(VK_HOME);
        keys[81] = is_pressed(VK_END);
        keys[82] = is_pressed(VK_INSERT);
    }

    if (window_data.message == WM_DEVICECHANGE)
    {
        is_check_for_new_device = true;
    }

    is_new_frame = false;
}

bool Input::GamepadVibrate(const float & left_motor_speed, const float & right_motor_speed) const
{
    if (!is_gamepad_connected)
        return false;

    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    vibration.wLeftMotorSpeed = static_cast<int>(Math::Clamp(left_motor_speed, 0.0f, 1.0f) * 65535);	// Convert [0, 1] to [0, 65535]
    vibration.wRightMotorSpeed = static_cast<int>(Math::Clamp(right_motor_speed, 0.0f, 1.0f) * 65535);	// Convert [0, 1] to [0, 65535]

    return XInputSetState(Input_Data::gamepad_num, &vibration) == ERROR_SUCCESS;
}
