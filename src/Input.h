#include "pch.h"

class Input {
    static inline const std::string filePath = "../assets/gamedata/input.mox";
public:
    static constexpr int MOUSE_LEFT = 0;
    static constexpr int MOUSE_RIGHT = 1;
    static constexpr int MOUSE_MIDDLE = 2;
    static constexpr int MOUSE_4 = 3;
    static constexpr int MOUSE_5 = 4;

    using Action = std::string; // or enum class

    // Set up default key bindings
    static void Initialize();

    static const bool SaveInputToFile();


    /// <summary>
    /// Keep in mind this overwrites current binds.
    /// </summary>
    static const bool LoadInputFromFile();

    static void ResetToDefaultBinds(bool saveToFile = false);

    // Bind a key to an action (replaces any existing bindings for this action)
    static void BindKey(const Action& action, sf::Keyboard::Scancode code);

    // Bind multiple keys to an action
    static void BindKeys(const Action& action, const std::vector<sf::Keyboard::Scancode>& codes);

#pragma region actions

    // Check if any key bound to an action is pressed
    static bool GetAction(const Action& action);

    // Check if any key bound to an action was just pressed
    static bool GetActionDown(const Action& action);

    // Check if any key bound to an action was just released
    static bool GetActionUp(const Action& action);
#pragma endregion

#pragma region key



    static bool GetKey(sf::Keyboard::Scancode code);

    static bool GetKeyDown(sf::Keyboard::Scancode code);

    static bool GetKeyUp(sf::Keyboard::Scancode code);
#pragma endregion

#pragma region Mouse
        // Mouse button support with integer codes
    static bool GetMouse(int button);

    static bool GetMouseDown(int button);

    static bool GetMouseUp(int button);

    static inline bool MouseMoved;
    static inline sf::Vector2i mousePos_Screen;
    static inline sf::Vector2f mousePos_World;
#pragma endregion

    // Original key-based methods
    static void Update();

    static void HandleEvent(const std::optional<sf::Event>& event);

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

