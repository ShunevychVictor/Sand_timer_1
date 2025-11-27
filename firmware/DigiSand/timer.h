#pragma once
#include <Arduino.h>

typedef void (*TimerCallback)();

class Timer {
   public:
   
    Timer() {}

    Timer(uint32_t ms, uint32_t seconds = 0, uint32_t minutes = 0, uint32_t hours = 0, uint32_t days = 0) {
        setTime(ms, seconds, minutes, hours, days);
        start();
    }

    // ========= JAVASCRIPT =========
    void setInterval(uint32_t ms) {
        prd = ms;
        start();
    }

    void setInterval(TimerCallback callback, uint32_t ms) {
        prd = ms;
        this->callback = callback;
        start();
    }

    void setTimeout(uint32_t ms) {
        prd = ms;
        start(true);
    }

    void setTimeout(TimerCallback callback, uint32_t ms) {
        prd = ms;
        this->callback = callback;
        start(true);
    }

    // =========== MANUAL ===========
    void setTime(uint32_t ms, uint32_t seconds = 0, uint32_t minutes = 0, uint32_t hours = 0, uint32_t days = 0) {
        prd = seconds;
        if (minutes) prd += minutes * 60ul;
        if (hours) prd += hours * 3600ul;
        if (days) prd += days * 86400ul;
        if (prd) prd *= 1000ul;
        prd += ms;
    }

    void start(bool once = false) {
        if (prd) {
            tmr = millis();
            mode = once ? 2 : 1;
        }
    }

    void stop() {
        mode = 0;
    }

    bool state() {
        return mode;
    }

    void attach(TimerCallback callback) {
        this->callback = callback;
    }

    void detach() {
        callback = nullptr;
    }

    bool tick() {
        if (mode && millis() - tmr >= prd) {
            if (callback) callback();
            if (mode == 1) start();
            else stop();
            return 1;
        }
        return 0;
    }
    operator bool() {
        return tick();
    }

   private:
    uint8_t mode = 0;  // 0 stop, 1 period, 2 timeout
    uint32_t tmr = 0, prd = 0;
    TimerCallback callback = nullptr;
};
