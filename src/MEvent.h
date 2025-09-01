#pragma once
#include "pch.h"

// Generic MEvent with arguments
template <typename... Args>
class MEvent {
public:
    using Callback = std::function<void(Args...)>;
    MEvent() = default;
     /// <returns>returns subscription ID, which can be used to unsubscribe a specific listener.</returns>
    size_t subscribe(Callback callback) {
        size_t id = nextId++;
        callbacks.push_back({ id, callback });
        return id;
    }




     /// <returns>returns subscription ID, which can be used to unsubscribe a specific listener.</returns>
    template <typename T>
    size_t subscribe(std::shared_ptr<T> instance, void (T::* method)(Args...)) {
        size_t id = nextId++;
        callbacks.push_back({
            id,
            [wptr = std::weak_ptr<T>(instance), method](Args... args) {
                if (auto sptr = wptr.lock()) {
                    (sptr.get()->*method)(args...);
                }
            }
            });
        return id;
    }
     /// <returns>returns subscription ID, which can be used to unsubscribe a specific listener.</returns>
    template <typename T>
    size_t subscribe(std::weak_ptr<T> instance, void (T::* method)(Args...)) {
        size_t id = nextId++;
        callbacks.push_back({
            id,
            [instance, method](Args... args) {
                if (auto sptr = instance.lock()) {
                    (sptr.get()->*method)(args...);
                }
            }
            });
        return id;
    }

    // Unsubscribe using the returned ID
    void unsubscribe(size_t id) {
        callbacks.erase(
            std::remove_if(callbacks.begin(), callbacks.end(),
                [id](const Subscription& s) { return s.id == id; }),
            callbacks.end()
        );
    }

    // Trigger the event
    void invoke(Args... args) {
        for (auto& s : callbacks) {
            s.callback(args...);
        }
    }
    void clear() {
        callbacks.clear();
    }

private:
    struct Subscription {
        size_t id;
        Callback callback;
    };

    std::vector<Subscription> callbacks;
    size_t nextId = 1;
};

// Specialization for parameter-less version
template <>
class MEvent<> {
public:
    using Callback = std::function<void()>;
    MEvent() = default;
    /// <returns>subscription ID, which can be used to unsubscribe a specific listener.</returns>
    size_t subscribe(Callback callback) {
        size_t id = nextId++;
        callbacks.push_back({ id, callback });
        return id;
    }
     /// <returns>subscription ID, which can be used to unsubscribe a specific listener.</returns>
    template <typename T>
    size_t subscribe(std::shared_ptr<T> instance, void (T::* method)()) {
        size_t id = nextId++;
        callbacks.push_back({
            id,
            [wptr = std::weak_ptr<T>(instance), method]() {
                if (auto sptr = wptr.lock()) {
                    (sptr.get()->*method)();
                }
            }
            });
        return id;
    }
     /// <returns>returns subscription ID, which can be used to unsubscribe a specific listener.</returns>
    template <typename T>
    size_t subscribe(std::shared_ptr<T> instance, void (T::* method)() const) {
        size_t id = nextId++;
        callbacks.push_back({
            id,
            [wptr = std::weak_ptr<T>(instance), method]() {
                if (auto sptr = wptr.lock()) {
                    (sptr.get()->*method)();
                }
            }
            });
        return id;
    }
    void unsubscribe(size_t id) {
        callbacks.erase(
            std::remove_if(callbacks.begin(), callbacks.end(),
                [id](const Subscription& s) { return s.id == id; }),
            callbacks.end()
        );
    }

    void invoke() {
        for (auto& s : callbacks) {
            s.callback();
        }
    }

    void clear() {
        callbacks.clear();
    }

private:
    struct Subscription {
        size_t id;
        Callback callback;
    };

    std::vector<Subscription> callbacks;
    size_t nextId = 1;
};
