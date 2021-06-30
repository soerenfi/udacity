#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive() {
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    // to wait for and receive new messages and pull them from the queue using move semantics.
    // The received object should then be returned by the receive function.
    std::unique_lock<std::mutex> lock(_mutex);

    // wait for a new message in queue
    _condVar.wait(lock, [this] { return !_queue.empty(); });

    T t = std::move(_queue.back());
    _queue.pop_back();
    return t;
}

template <typename T>
void MessageQueue<T>::send(T&& msg) {
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.clear();
    _queue.push_back(std::move(msg));
    _condVar.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::red; }

void TrafficLight::waitForGreen() {
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop
    // runs and repeatedly calls the receive function on the message queue.
    // Once it receives TrafficLightPhase::green, the method returns.
    while (true) {
        if (_queue.receive() == TrafficLightPhase::green) {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate() {
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method
    // „simulate“ is called. To do this, use the thread queue in the base class.
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles
    // and toggles the current phase of the traffic light between red and green and sends an update method
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds.
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
    auto start = std::chrono::high_resolution_clock::now();
    // from https://diego.assencio.com/?index=6890b8c50169ef45b74db135063c227c
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(4, 6);
    auto cycle = std::chrono::milliseconds(distribution(generator) * 1000);

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start) > cycle) {
            _currentPhase =
                (_currentPhase == TrafficLightPhase::red ? _currentPhase = TrafficLightPhase::green
                                                         : TrafficLightPhase::red);
            _queue.send(std::move(_currentPhase));

            start = now;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
