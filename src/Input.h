#include <SFML/Window.hpp>
#include <unordered_map>
#include <string>
#include <vector>

class Input {
public:
    using Action = std::string; // or enum class

    // Set up default key bindings
    static void Initialize() {
        s_actionToKeys.clear();

        // Example default bindings
        BindKey("up", sf::Keyboard::Scan::W);
        BindKey("down", sf::Keyboard::Scan::S);
        BindKey("left", sf::Keyboard::Scan::A);       
        BindKey("right", sf::Keyboard::Scan::D);
    }

    // Bind a key to an action (replaces any existing bindings for this action)
    static void BindKey(const Action& action, sf::Keyboard::Scancode code) {
        s_actionToKeys[action] = { code };
    }

    // Bind multiple keys to an action
    static void BindKeys(const Action& action, const std::vector<sf::Keyboard::Scancode>& codes) {
        s_actionToKeys[action] = codes;
    }

    // Check if any key bound to an action is pressed
    static bool GetAction(const Action& action) {
        auto it = s_actionToKeys.find(action);
        if (it != s_actionToKeys.end()) {
            for (auto key : it->second) {
                if (GetKey(key)) return true;
            }
        }
        return false;
    }

    // Check if any key bound to an action was just pressed
    static bool GetActionDown(const Action& action) {
        auto it = s_actionToKeys.find(action);
        if (it != s_actionToKeys.end()) {
            for (auto key : it->second) {
                if (GetKeyDown(key)) return true;
            }
        }
        return false;
    }

    // Check if any key bound to an action was just released
    static bool GetActionUp(const Action& action) {
        auto it = s_actionToKeys.find(action);
        if (it != s_actionToKeys.end()) {
            for (auto key : it->second) {
                if (GetKeyUp(key)) return true;
            }
        }
        return false;
    }

    // Original key-based methods
    static void Update() {
        for (auto& [key, state] : s_keyStates) {
            if (state == KeyState::JustPressed) state = KeyState::Pressed;
            else if (state == KeyState::JustReleased) state = KeyState::Released;
        }
    }

    static void HandleEvent(const std::optional<sf::Event>& event) {
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            bool isRepeat = (s_keyStates[keyEvent->scancode] == KeyState::Pressed ||
                s_keyStates[keyEvent->scancode] == KeyState::JustPressed);

            if (!isRepeat) {
                s_keyStates[keyEvent->scancode] = KeyState::JustPressed;
            }
        }
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
            s_keyStates[keyEvent->scancode] = KeyState::JustReleased;
        }
    }

    static bool GetKey(sf::Keyboard::Scancode code) {
        auto it = s_keyStates.find(code);
        return it != s_keyStates.end() &&
            (it->second == KeyState::Pressed || it->second == KeyState::JustPressed);
    }

    static bool GetKeyDown(sf::Keyboard::Scancode code) {
        auto it = s_keyStates.find(code);
        return it != s_keyStates.end() && it->second == KeyState::JustPressed;
    }

    static bool GetKeyUp(sf::Keyboard::Scancode code) {
        auto it = s_keyStates.find(code);
        return it != s_keyStates.end() && it->second == KeyState::JustReleased;
    }

private:
    enum class KeyState {
        Released,
        JustPressed,
        Pressed,
        JustReleased
    };

    static inline std::unordered_map<sf::Keyboard::Scancode, KeyState> s_keyStates;
    static inline std::unordered_map<Action, std::vector<sf::Keyboard::Scancode>> s_actionToKeys;
};

