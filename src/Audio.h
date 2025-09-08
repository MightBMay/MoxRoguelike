#include "pch.h"

class Audio {
private:
    // Private constructor and destructor
    Audio() = default;
    ~Audio() = default;

    // Delete copy constructor and assignment operator
    Audio(const Audio&) = delete;
    Audio& operator=(const Audio&) = delete;

    // FMOD System pointer
    FMOD::System* pSystem = nullptr;
    FMOD::Studio::System* pStudioSystem = nullptr; // Added Studio System

    std::unordered_map<std::string, FMOD::Studio::Bank*> loadedBanks;
   

public:
    // Static method to get the single instance (Meyer's Singleton)
    static Audio& get() {
        static Audio instance;
        return instance;
    }

    // Initialize the audio system
    void Initialize() {
        FMOD_RESULT result;

        // Initialize the Core System (for low-level audio)
        result = FMOD::System_Create(&pSystem);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: System_Create failed - " << FMOD_ErrorString(result) << std::endl;
            return;
        }

        result = pSystem->init(512, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: System init failed - " << FMOD_ErrorString(result) << std::endl;
            return;
        }

        // Initialize the Studio System (for FMOD Studio events and banks)
        result = FMOD::Studio::System::create(&pStudioSystem);
        if (result != FMOD_OK) {
            std::cerr << "FMOD Studio error: System create failed - " << FMOD_ErrorString(result) << std::endl;
            return;
        }

        result = pStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            std::cerr << "FMOD Studio error: System init failed - " << FMOD_ErrorString(result) << std::endl;
            return;
        }

        std::cout << "Audio system initialized successfully." << std::endl;
    }

    // Update the audio system (MUST be called once per frame)
    void Update() {
        if (pStudioSystem) {
            pStudioSystem->update();
        }
        // The core system (pSystem) is updated by the studio system
    }

    // Clean up and shutdown
    void Shutdown() {
        if (pStudioSystem) {
            pStudioSystem->release();
            pStudioSystem = nullptr;
        }
        if (pSystem) {
            pSystem->release();
            pSystem = nullptr;
        }
        std::cout << "Audio system shutdown." << std::endl;
    }

    // Getter for the core system (for low-level audio)
    FMOD::System* GetSystem() const {
        assert(pSystem != nullptr && "Audio system not initialized!");
        return pSystem;
    }

    // Getter for the studio system (for FMOD Studio events)
    FMOD::Studio::System* GetStudioSystem() const {
        assert(pStudioSystem != nullptr && "Audio Studio system not initialized!");
        return pStudioSystem;
    }

    // Example method to load a bank (FMOD Studio)
    void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL) {     
        static const std::string fmod_bank_path = "../assets/audio/banks/";
        if (!pStudioSystem) return;

        std::string fullPath = fmod_bank_path + bankName;

        FMOD::Studio::Bank* bank = nullptr;
        FMOD_RESULT result = pStudioSystem->loadBankFile(fullPath.c_str(), flags, &bank);

        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to load bank '" << bankName << "' - " << FMOD_ErrorString(result) << std::endl;
        }
        else {
            std::cout << "Loaded bank: " << bankName << std::endl;
        }
    }


    void UnloadBank(const std::string& bankName) {
        auto it = loadedBanks.find(bankName);
        if (it != loadedBanks.end()) {
            it->second->unload();
            loadedBanks.erase(it);
            std::cout << "\nUnloaded bank: " << bankName;
        }
    }

    // Example method to play a simple sound (low-level)
    void PlaySound(FMOD::Sound* sound) {
        if (!pSystem || !sound) return;

        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = pSystem->playSound(sound, nullptr, false, &channel);

        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to play sound - " << FMOD_ErrorString(result) << std::endl;
        }
    }
};