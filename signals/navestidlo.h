#ifndef NAVESTIDLO_H
#define NAVESTIDLO_H

#include "navestidla_config.h"
#include "znak.h"

typedef struct {
    int zluta_up;
    int zelena;
    int cervena;
    int bila;
    int zluta_down;
    int zeleny_prouzek_up;
    int zluty_prouzek;
    int zeleny_prouzek_down;
    int modra;
} Navestidlo_pin_config;

class Pin_navestidla {
    private:
        int pin;
        int hodnota;

    public:
        Pin_navestidla() {
            this->pin = -1;
            this->hodnota = LED_OFF;
        }

        void set_pin(int pin) {
            this->pin = pin;
        }

        int set_hodnota(int hodnota) {
            if (this->pin == -1) return -1;
            this->hodnota = hodnota;
            return 0;
        }

        int get_hodnota() {
            return this->hodnota;
        }

        void fill_buffer(int* buffer, int length) {
            if (length != COUNT_LEDS) return;

            if (this->pin == -1) return;

            buffer[this->pin] = this->hodnota;
        }
};

class Navestidlo {
    private:
        Pin_navestidla cervena;
        Pin_navestidla modra;
        Pin_navestidla zelena;
        Pin_navestidla zluta_up;
        Pin_navestidla zluta_down;
        Pin_navestidla bila;
        Pin_navestidla zeleny_prouzek_up;
        Pin_navestidla zluty_prouzek;
        Pin_navestidla zeleny_prouzek_down;

        int permissive_pin = -1;

        void clear_pins();

    public:
        Navestidlo(const Navestidlo_pin_config *config); 
        Navestidlo();

        void set_from_config(const Navestidlo_pin_config *config);
        void fillBuffer(int* buffer, int length);

        void setZkracenaBrzda();
        int setZnak(int i2c_code);
        int get_permissive_pin();
};

#endif