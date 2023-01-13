#pragma once
#include "Core/Subsystems/ISubsystem.h"

enum KeyCode
{
    // Keyboard
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,/*Function*/
    Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,/*Numbers*/
    Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,/*Numpad*/
    Q, W, E, R, T, Y, U, I, O, P,/*Letters*/
    A, S, D, F, G, H, J, K, L,
    Z, X, C, V, B, N, M,
    Esc,/*Controls*/
    Tab,
    Shift_Left, Shift_Right,
    Ctrl_Left, Ctrl_Right,
    Alt_Left, Alt_Right,
    Space,
    CapsLock,
    Backspace,
    Enter,
    Delete,
    Arrow_Left, Arrow_Right, Arrow_Up, Arrow_Down,
    Page_Up, Page_Down,
    Home,
    End,
    Insert,

    // Mouse
    Click_Left,
    Click_Middle,
    Click_Right,

    // Gamepad
    DPad_Up,
    DPad_Down,
    DPad_Left,
    DPad_Right,
    Button_A,
    Button_B,
    Button_X,
    Button_Y,
    Start,
    Back,
    Left_Thumb,
    Right_Thumb,
    Left_Shoulder,
    Right_Shoulder
};

class Input final : public ISubsystem
{
public:
    Input(class Context* const context);
    ~Input() = default;

    bool Initialize() override;
    void Update(const float& delta_time) override;

    void OnWindowData();

    //Keys
    bool GetKey(const KeyCode& key)     { return keys[static_cast<uint>(key)]; }
    bool GetKeyDown(const KeyCode& key) { return GetKey(key) && !keys_previous_frame[static_cast<uint>(key)]; }
    bool GetKeyUp(const KeyCode& key)   { return !GetKey(key) && keys_previous_frame[static_cast<uint>(key)]; }

    //Mouse
    auto GetMousePosition() const -> const D3DXVECTOR2& { return mouse_position; }
    auto GetMouseDelta() const -> const D3DXVECTOR2& { return mouse_delta; }

    //Gamepad
    bool IsGamepadConnected() const { return is_gamepad_connected; }
    auto GetGamepadThumbStickLeft() const -> const D3DXVECTOR2& { return gamepad_thumb_left; }
    auto GetGamepadThumbStickRight() const -> const D3DXVECTOR2& { return gamepad_thumb_right; }
    auto GetGamepadTriggerLeft() const -> const float& { return gamepad_trigger_left; }
    auto GetGamepadTriggerRight() const -> const float& { return gamepad_trigger_right; }
    bool GamepadVibrate(const float& left_motor_speed, const float& right_motor_speed) const;

private:
    //Keys
    std::array<bool, 99> keys;
    std::array<bool, 99> keys_previous_frame;
    uint start_index_mouse = 83;
    uint start_index_gamepad = 86;

    //Mouse
    D3DXVECTOR2 mouse_position = D3DXVECTOR2{};
    D3DXVECTOR2 mouse_delta = D3DXVECTOR2{};
    int mouse_wheel = 0;
    float mouse_wheel_delta = 0;

    //Gamepad
    bool is_gamepad_connected = false;
    D3DXVECTOR2 gamepad_thumb_left = D3DXVECTOR2{};
    D3DXVECTOR2 gamepad_thumb_right = D3DXVECTOR2{};
    float gamepad_trigger_left = 0.0f;
    float gamepad_trigger_right = 0.0f;

    //Mics
    bool is_new_frame = false;
    bool is_check_for_new_device = false;
};