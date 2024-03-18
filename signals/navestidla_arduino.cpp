#include "navestidla_arduino.h"

#ifndef ESP8266

int old_buffer[COUNT_LEDS];

bool live_bit_error = false;

unsigned long last_live_bit = millis();
auto timer = timer_create_default();

auto led00 = JLed(0).On(); // pos 1
auto led01 = JLed(1).On(); // pos 1

auto led02 = JLed(2).On(); // pos 2
auto led03 = JLed(3).On(); // pos 2
auto led04 = JLed(4).On(); // pos 2
auto led05 = JLed(5).On(); // pos 2
auto led06 = JLed(6).On(); // pos 2
auto led07 = JLed(7).On(); // pos 2

auto led08 = JLed(8).On(); // pos 3
auto led09 = JLed(9).On(); // pos 3
auto led10 = JLed(10).On(); // pos 3
auto led11 = JLed(11).On(); // pos 3
auto led12 = JLed(12).On(); // pos 3

auto led13 = JLed(13).On(); // pos 1
auto led14 = JLed(14).On(); // pos 1
auto led15 = JLed(15).On(); // pos 1
auto led16 = JLed(16).On(); // pos 1
auto led17 = JLed(17).On(); // pos 1

const int count_leds = 18;

JLed* leds[count_leds] = {
    &led00,
    &led01,
    // nullptr,
    // nullptr,
    &led02,
    &led03,
    &led04,
    &led05,
    &led06,
    &led07,
    &led08,
    &led09,
    &led10,
    &led11,
    &led12,
    &led13,
    &led14,
    &led15,
    &led16,
    &led17
};

void clear_buffer(int howMany)
{
    for (int i = 0; i < howMany; i++) int tmp = Wire.read();
}

void show_leds()
{
    for (int i = 0; i < count_leds; i++)
    {
        uint8_t show_on_led = Wire.read();
        if (TURN_ON_SERIAL) Serial.print(show_on_led);

        if (old_buffer[i] == show_on_led) continue;
        old_buffer[i] = show_on_led;

        switch (show_on_led)
        {
        case LED_ON:
            turn_on(leds[i]);
            break;
        case LED_OFF:
            turn_off(leds[i]);
            break;
        case LED_BLINK_SLOW:
            breathe_slow(leds[i]);
            break;
        case LED_BLINK_FAST:
            breathe_fast(leds[i]);
            break;
        
        default:
            if (TURN_ON_SERIAL) Serial.println("unknown led state");
            break;
        }
    }
}

void turn_on(JLed* led) {
    if (led != nullptr) {
        // turnoff was added for removing the previous effect
        turn_off(led);
        led->DelayBefore(delay_fade_off).FadeOff(fade_on_time);
    }
}

void turn_off(JLed* led) {
    if (led != nullptr) {
        led->FadeOn(fade_off_time).Repeat(1);
    }
}

void breathe_slow(JLed* led) {
    if (led != nullptr) {
        turn_off(led);
        led->Breathe(150, slow_period, 150).DelayAfter(slow_period).Forever();
    }
}

void breathe_fast(JLed* led) {
    if (led != nullptr) {
        turn_off(led);
        led->Breathe(50, fast_period, 50).DelayAfter(fast_period).Forever();
    }
}

void map_on_leds(void (*f)(JLed* led)) {
    for (int i = 0; i < count_leds; i++) {
        old_buffer[i] = -1;
        f(leds[i]);
    }
}

bool handle_live_bit(void *)
{
    if (millis() - last_live_bit >= LIVE_BIT_DELAY + LIVE_BIT_SLACK)
    {
        if (TURN_ON_SERIAL) Serial.println("esp not reachable");
        if (!live_bit_error)
        {
            map_on_leds(turn_on);
        }
        live_bit_error = true;
    }
    else
    {
        if (live_bit_error)
        {
            // TODO maybe have an array where the esp sends which leds are permissive
            map_on_leds(turn_off);
            if (TURN_ON_SERIAL) Serial.println("esp reachable again, setting permissive on navestidlo");
        }
        live_bit_error = false;
    }

    last_live_bit = millis();
    return true;
}

void requestEvent()
{
    if (TURN_ON_SERIAL) Serial.println("sending live bit");
    Wire.write(LIVE_BIT);
}

void receiveEvent(int howMany)
{
    Serial.print("got ");
    Serial.println(howMany);

    last_live_bit = millis(); // reset live bit

    if (howMany == 1)
    {
        if (TURN_ON_SERIAL) Serial.println("got live bit");
        int live_bit = Wire.read();
        if (live_bit_error)
        {
            if (TURN_ON_SERIAL) Serial.println("esp reachable again, setting permissive on navestidlo");
            // TODO set permissive
            map_on_leds(turn_off);
            live_bit_error = false;
        }
        return;
    }
    if (howMany == count_leds)
    {
        if (TURN_ON_SERIAL) Serial.print("update requested");

        if (live_bit_error)
        {                             // new znak => clear the error
            last_live_bit = millis(); // reset live bit
            map_on_leds(turn_off);
            if (TURN_ON_SERIAL) Serial.println("got bits, should show them");
            live_bit_error = false;
        }

        show_leds();
        return;
    }

    if (TURN_ON_SERIAL) Serial.println("clearing buffer, unknown number of bytes");
    clear_buffer(howMany);
}

void navestidla_setup(uint8_t bus_address)
{
    // delay to wait for the esp to start
    delay(ARDUINO_STARTUP_DELAY);
    Wire.begin(bus_address);
    Wire.onReceive(receiveEvent); // function that executes whenever data is received from writer
    Wire.onRequest(requestEvent); // function that executes whenever data is requested from reader
    if (TURN_ON_SERIAL) {
        Serial.begin(115200); // start serial communication at 9600bps
    }
    timer.every(LIVE_BIT_DELAY + LIVE_BIT_SLACK, handle_live_bit);
    // TODO init the leds
    for (int i = 0; i < count_leds; i++) {
        old_buffer[i] = -1;
    }
}

void navestidla_loop()
{
    for (int i = 0; i < count_leds; i++)
    {
        leds[i]->Update();
    }
    timer.tick();
}

#endif // ESP8266