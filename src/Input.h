#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <unordered_map>
#include <string>
#include <vector>

class Input {
public:
    static constexpr int MOUSE_LEFT = 0;
    static constexpr int MOUSE_RIGHT = 1;
    static constexpr int MOUSE_MIDDLE = 2;
    static constexpr int MOUSE_4 = 3;
    static constexpr int MOUSE_5 = 4;

    using Action = std::string; // or enum class

    // Set up default key bindings
    static void Initialize() {
        _actionToKeys.clear();

        // Example default bindings
        BindKey("up", sf::Keyboard::Scan::W);
        BindKey("down", sf::Keyboard::Scan::S);
        BindKey("left", sf::Keyboard::Scan::A);       
        BindKey("right", sf::Keyboard::Scan::D);
        BindKey("ability1", sf::Keyboard::Scan::Q);
        BindKey("ability2", sf::Keyboard::Scan::E);
    }

    // Bind a key to an action (replaces any existing bindings for this action)
    static void BindKey(const Action& action, sf::Keyboard::Scancode code) {
        _actionToKeys[action] = { code };
    }

    // Bind multiple keys to an action
    static void BindKeys(const Action& action, const std::vector<sf::Keyboard::Scancode>& codes) {
        _actionToKeys[action] = codes;
    }
#pragma region actions

    // Check if any key bound to an action is pressed
    static bool GetAction(const Action& action) {
        auto it = _actionToKeys.find(action);
        if (it != _actionToKeys.end()) {
            for (auto key : it->second) {
                if (GetKey(key)) return true;
            }
        }
        return false;
    }

    // Check if any key bound to an action was just pressed
    static bool GetActionDown(const Action& action) {
        auto it = _actionToKeys.find(action);
        if (it != _actionToKeys.end()) {
            for (auto key : it->second) {
                if (GetKeyDown(key)) return true;
            }
        }
        return false;
    }

    // Check if any key bound to an action was just released
    static bool GetActionUp(const Action& action) {
        auto it = _actionToKeys.find(action);
        if (it != _actionToKeys.end()) {
            for (auto key : it->second) {
                if (GetKeyUp(key)) return true;
            }
        }
        return false;
    }
#pragma endregion

#pragma region key



    static bool GetKey(sf::Keyboard::Scancode code) {
        auto it = _keyStates.find(code);
        return it != _keyStates.end() &&
            (it->second == KeyState::Pressed || it->second == KeyState::JustPressed);
    }

    static bool GetKeyDown(sf::Keyboard::Scancode code) {
        auto it = _keyStates.find(code);
        return it != _keyStates.end() && it->second == KeyState::JustPressed;
    }

    static bool GetKeyUp(sf::Keyboard::Scancode code) {
        auto it = _keyStates.find(code);
        return it != _keyStates.end() && it->second == KeyState::JustReleased;
    }
#pragma endregion

#pragma region Mouse
        // Mouse button support with integer codes
    static bool GetMouse(int button) {
        auto it = _mouseStates.find(button);
        return it != _mouseStates.end() &&
            (it->second == KeyState::Pressed || it->second == KeyState::JustPressed);
    }

    static bool GetMouseDown(int button) {
        auto it = _mouseStates.find(button);
        return it != _mouseStates.end() && it->second == KeyState::JustPressed;
    }

    static bool GetMouseUp(int button) {
        auto it = _mouseStates.find(button);
        return it != _mouseStates.end() && it->second == KeyState::JustReleased;
    }
#pragma endregion

    // Original key-based methods
    static void Update() {
        for (auto& [key, state] : _keyStates) {
            if (state == KeyState::JustPressed) state = KeyState::Pressed;
            else if (state == KeyState::JustReleased) state = KeyState::Released;
        }

        for (auto& [button, state] : _mouseStates) {
            if (state == KeyState::JustPressed) state = KeyState::Pressed;
            else if (state == KeyState::JustReleased) state = KeyState::Released;
        }
    }

    static void HandleEvent(const std::optional<sf::Event>& event) {
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            bool isRepeat = (_keyStates[keyEvent->scancode] == KeyState::Pressed ||
                _keyStates[keyEvent->scancode] == KeyState::JustPressed);

            if (!isRepeat) {
                _keyStates[keyEvent->scancode] = KeyState::JustPressed;
            }
        }
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
            _keyStates[keyEvent->scancode] = KeyState::JustReleased;
        }

        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            int button = -1;
            switch (mouseEvent->button) {
                case sf::Mouse::Button::Left:   button = MOUSE_LEFT; break;
                case sf::Mouse::Button::Right:  button = MOUSE_RIGHT; break;
                case sf::Mouse::Button::Middle: button = MOUSE_MIDDLE; break;
                case sf::Mouse::Button::Extra1: button = MOUSE_4; break;
                case sf::Mouse::Button::Extra2: button = MOUSE_5; break;
                default: break;
            }

            if (button != -1) {
                bool isRepeat = (_mouseStates[button] == KeyState::Pressed ||
                    _mouseStates[button] == KeyState::JustPressed);
                if (!isRepeat) {
                    _mouseStates[button] = KeyState::JustPressed;
                }
            }
        }
        else if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
            int button = -1;
            switch (mouseEvent->button) {
                case sf::Mouse::Button::Left:   button = MOUSE_LEFT; break;
                case sf::Mouse::Button::Right:  button = MOUSE_RIGHT; break;
                case sf::Mouse::Button::Middle: button = MOUSE_MIDDLE; break;
                case sf::Mouse::Button::Extra1: button = MOUSE_4; break;
                case sf::Mouse::Button::Extra2: button = MOUSE_5; break;
                default: break;
            }

            if (button != -1) {
                _mouseStates[button] = KeyState::JustReleased;
            }
        }

    }

private:
    enum class KeyState {
        Released,
        JustPressed,
        Pressed,
        JustReleased
    };

    static inline std::unordered_map<sf::Keyboard::Scancode, KeyState> _keyStates;
    static inline std::unordered_map<int, KeyState> _mouseStates;
    static inline std::unordered_map<Action, std::vector<sf::Keyboard::Scancode>> _actionToKeys;
};

