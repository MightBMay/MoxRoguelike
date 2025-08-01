#pragma once
#include "MEvent.h"
#include <iostream>
class Timer {
public:
    Timer(float duration, bool looping = false) :duration(duration), isLooping(looping) {}
    /// <summary>
    /// starts the timer from whatever duration was set. if the timer is already running, restarts the timer.
    /// </summary>
    inline void start(bool forceStart = false) {
        if (isStarted && !forceStart) return; // exit early to prevent restarting if the timer is already runnign
        remainingDuration = duration;
        isStarted = true;
        startEvent.invoke();
    }


    void update(float deltaTime) {
        if (!isStarted) return; // ignore if not started.

        if (remainingDuration <= 0) // end if timer finished (stops timer)
            onEnd();

        else // decrement timer.
            remainingDuration -= deltaTime;
    }


    void onEnd() {

        endEvent.invoke();

        if (isLooping) {
            start();
        }
        else {
            isStarted = false;
            remainingDuration = 0;
        }
    }

    inline void setLooping(bool value) { isLooping = value; }
    inline void cancel() { isStarted = false; remainingDuration = 0; }
    inline void pause() {  isStarted = false; }
    inline void resume() {
        if (remainingDuration <= 0) return;
        isStarted = true;
    }

    inline MEvent<>& getStartEvent() { return startEvent; }
    inline MEvent<>& getEndEvent() { return endEvent; }

protected:
    MEvent<> startEvent{};
    MEvent<> endEvent{};

private:
    float remainingDuration = 0;
    const float duration;
    bool isLooping = false;
    bool isStarted = false;
};