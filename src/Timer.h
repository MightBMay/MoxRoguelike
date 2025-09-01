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

        if (isLooping) {
            loopEvent.invoke();
            start(true);
        }
        else {
            isStarted = false;
            remainingDuration = 0;
            endEvent.invoke();
        }
    }
    inline bool inProgress() { return remainingDuration >= 0 && isStarted; }
    inline void setLooping(bool value) { isLooping = value; }
    inline void cancel() { isStarted = false; remainingDuration = duration; }
    inline void pause() {  isStarted = false; }
    inline void resume() {
        if (remainingDuration <= 0) return;
        isStarted = true;
    }

    inline void clear() {
        startEvent.clear();
        loopEvent.clear();
        endEvent.clear();
        isStarted = false;
        remainingDuration = duration;
    }

    /// <summary>
    /// Gets the start event. Invoked any time the timer starts, including Looping timers.
    /// </summary>
    inline MEvent<>& getStartEvent() { return startEvent; }
    /// <summary>
    /// Gets the loop event. Invoked when the timer ends, and looping is ON.
    /// </summary>
    inline MEvent<>& getLoopEvent() { return loopEvent; }
    /// <summary>
    /// gets the end event. only invoked when the timer ends and looping is OFF.
    /// </summary>
    inline MEvent<>& getEndEvent() { return endEvent; }

protected:
    MEvent<> startEvent{};
    MEvent<> loopEvent{};
    MEvent<> endEvent{};

private:
    float remainingDuration = 0;
    const float duration;
    bool isLooping = false;
    bool isStarted = false;
};