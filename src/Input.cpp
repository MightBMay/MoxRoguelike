#include "pch.h"


void Input::Initialize() {
    if (!LoadInputFromFile()) 
        ResetToDefaultBinds(true);
    
}

const bool Input::SaveInputToFile() {
    json savedData; // create empty json object.
    for (const auto& [action, scanCodes] : _actionToKeys) { // iterate action to keys array.

        json keyArray = json::array(); // create empty json array and populate with int value of key scancodes
        for (auto& code : scanCodes)
            keyArray.push_back(static_cast<int>(code));

        savedData[action] = keyArray;

    }

    try
    {
        std::ofstream file(filePath); // open file and output data to it.
        file << savedData.dump(4);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving input config: " << e.what() << std::endl;
        return false;
    }

}

 const bool Input::LoadInputFromFile() {
    try {
        std::ifstream file(filePath); // open file
        if (!file.is_open()) {
            std::cerr << "\nError opening input.mox  at: " << filePath;
            return false;
        }

        json j; // make empty json object and output input file to it.
        file >> j;
        _actionToKeys.clear(); // clear current action bindings
        for (auto it = j.begin(); it != j.end(); ++it) { // iterate json object
            const std::string action = it.key(); // get key and value
            const json& keyArray = it.value();

            std::vector<sf::Keyboard::Scancode> scancodes; // create vector to assign to map's value.
            for (const auto& keyValue : keyArray) // populate vector with whatever keys were in json.
                scancodes.push_back(static_cast<sf::Keyboard::Scancode>(keyValue.get<int>()));

            _actionToKeys[action] = scancodes; // assign keys to the action.
        }
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Error loading input config: " << e.what() << std::endl;
        return false;
    }



}

 void Input::ResetToDefaultBinds(bool saveToFile) {
     _actionToKeys.clear();
     BindKey("up", sf::Keyboard::Scan::W);
     BindKey("down", sf::Keyboard::Scan::S);
     BindKey("left", sf::Keyboard::Scan::A);
     BindKey("right", sf::Keyboard::Scan::D);
     BindKey("ability1", sf::Keyboard::Scan::Q);
     BindKey("ability2", sf::Keyboard::Scan::E);
     if (saveToFile) SaveInputToFile();

 }

     // Bind a key to an action (replaces any existing bindings for this action)
 void Input::BindKey(const Action& action, sf::Keyboard::Scancode code) {
     _actionToKeys[action] = { code };
 }

 // Bind multiple keys to an action
 void Input::BindKeys(const Action& action, const std::vector<sf::Keyboard::Scancode>& codes) {
     _actionToKeys[action] = codes;
 }


#pragma region actions

   // Check if any key bound to an action is pressed
 bool Input::GetAction(const Action& action) {
     auto it = _actionToKeys.find(action);
     if (it != _actionToKeys.end()) {
         for (auto key : it->second) {
             if (GetKey(key)) return true;
         }
     }
     return false;
 }

 // Check if any key bound to an action was just pressed
 bool Input::GetActionDown(const Action& action) {
     auto it = _actionToKeys.find(action);
     if (it != _actionToKeys.end()) {
         for (auto key : it->second) {
             if (GetKeyDown(key)) return true;
         }
     }
     return false;
 }

 // Check if any key bound to an action was just released
 bool Input::GetActionUp(const Action& action) {
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



 bool Input::GetKey(sf::Keyboard::Scancode code) {
     auto it = _keyStates.find(code);
     return it != _keyStates.end() &&
         (it->second == KeyState::Pressed || it->second == KeyState::JustPressed);
 }

 bool Input::GetKeyDown(sf::Keyboard::Scancode code) {
     auto it = _keyStates.find(code);
     return it != _keyStates.end() && it->second == KeyState::JustPressed;
 }

 bool Input::GetKeyUp(sf::Keyboard::Scancode code) {
     auto it = _keyStates.find(code);
     return it != _keyStates.end() && it->second == KeyState::JustReleased;
 }
#pragma endregion

#pragma region Mouse
        // Mouse button support with integer codes
 bool Input::GetMouse(int button) {
     auto it = _mouseStates.find(button);
     return it != _mouseStates.end() &&
         (it->second == KeyState::Pressed || it->second == KeyState::JustPressed);
 }

 bool Input::GetMouseDown(int button) {
     auto it = _mouseStates.find(button);
     return it != _mouseStates.end() && it->second == KeyState::JustPressed;
 }

 bool Input::GetMouseUp(int button) {
     auto it = _mouseStates.find(button);
     return it != _mouseStates.end() && it->second == KeyState::JustReleased;
 }
#pragma endregion

    // Original key-based methods
 void Input::Update() {
     for (auto& [key, state] : _keyStates) {
         if (state == KeyState::JustPressed) state = KeyState::Pressed;
         else if (state == KeyState::JustReleased) state = KeyState::Released;
     }

     for (auto& [button, state] : _mouseStates) {
         if (state == KeyState::JustPressed) state = KeyState::Pressed;
         else if (state == KeyState::JustReleased) state = KeyState::Released;
     }
 }

 void Input::HandleEvent(const std::optional<sf::Event>& event) {
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
