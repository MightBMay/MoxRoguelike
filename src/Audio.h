#include "pch.h"

class Audio {
private:
    
    struct BankEntry {
        FMOD::Studio::Bank* bank;
        size_t referenceCount = 1; 
        BankEntry(FMOD::Studio::Bank* bank) :bank(bank){}
    };

    // Private constructor and destructor
    Audio() = delete;
    ~Audio() = delete;

    // Delete copy constructor and assignment operator
    Audio(const Audio&) = delete;
    Audio& operator=(const Audio&) = delete;

    // FMOD System pointer
    static inline FMOD::System* pSystem = nullptr;
    static inline FMOD::Studio::System* pStudioSystem = nullptr; // Added Studio System
    static inline std::unordered_map<std::string, BankEntry> loadedBanks;
    static inline const std::string fmod_bank_path = "../assets/audio/banks/";

public:
    static void Initialize() {
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
        LoadBank("master.bank");
        LoadBank("master.strings.bank");

        LoadBank("enemies.bank");
        LoadBank("player.bank");
        LoadBank("ui.bank");
        LoadBank("weapons.bank");


        std::cout << "Audio system initialized successfully." << std::endl;
    }

    // update audio system (MUST be called once per frame)
    static void Update() {
        if (pStudioSystem) {
            pStudioSystem->update();
        }
        // The core system (pSystem) is updated by the studio system
    }

    static void Shutdown() {
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
    static FMOD::System* GetSystem() {
        assert(pSystem != nullptr && "Audio system not initialized!");
        return pSystem;
    }

    // Getter for the studio system (for FMOD Studio events)
    static FMOD::Studio::System* GetStudioSystem() {
        assert(pStudioSystem != nullptr && "Audio Studio system not initialized!");
        return pStudioSystem;
    }

    
    static void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL) {     
        if (!pStudioSystem) return;
        std::string fullPath = fmod_bank_path + bankName;
        
        // check if bank already loaded.
        auto it = loadedBanks.find(bankName);
        if (it != loadedBanks.end()) {
            it->second.referenceCount++;
            return;
        }

        FMOD::Studio::Bank* bank = nullptr;
        FMOD_RESULT result = pStudioSystem->loadBankFile(fullPath.c_str(), flags, &bank);

        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to load bank '" << bankName << "' - " << FMOD_ErrorString(result) << std::endl;
            
        }
        else {
            std::cout << "Loaded bank: " << bankName << std::endl;
            loadedBanks.emplace(bankName, bank);
        }
    }

    static void UnloadBank(const std::string& bankName) {
        auto it = loadedBanks.find(bankName);
        if (it != loadedBanks.end()) {
            it->second.referenceCount--; // decrement reference count
            if (it->second.referenceCount >= 1) return; // if bank still referenced, don't unload.

            it->second.bank->unload();
            loadedBanks.erase(it);
            std::cout << "\nUnloaded bank: " << bankName;
        }
    }

    static FMOD::Studio::EventInstance* CreateFMODEvent(const std::string& eventPath) {
        if (!pStudioSystem) return nullptr;

        FMOD::Studio::EventDescription* eDescription = nullptr;
        FMOD_RESULT result = pStudioSystem->getEvent(eventPath.c_str(), &eDescription);
        if(result != FMOD_OK) {
            std::cerr << "Error getting event: " << eventPath << " - " << FMOD_ErrorString(result) << std::endl;
            return nullptr;
        }

        FMOD::Studio::EventInstance* eInstance = nullptr;
        result = eDescription->createInstance(&eInstance);
        if (result != FMOD_OK) {
            std::cerr << "Error creating event instance: " << eventPath << " - " << FMOD_ErrorString(result) << std::endl;
            return nullptr;
        }

        return eInstance; // Caller now owns this instance and must release it!

    }

    static void PlayOneShot(const std::string& eventPath) {
        auto eInstance = CreateFMODEvent(eventPath);
        if (eInstance == nullptr) return;
        eInstance->start();
        eInstance->release();

    }

    // Example method to play a simple sound (low-level)
    static void PlaySound(FMOD::Sound* sound) {
        if (!pSystem || !sound) return;

        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = pSystem->playSound(sound, nullptr, false, &channel);

        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to play sound - " << FMOD_ErrorString(result) << std::endl;
        }
    }
};