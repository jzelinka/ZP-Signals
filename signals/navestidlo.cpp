#include "navestidlo.h"

Navestidlo::Navestidlo(const Navestidlo_pin_config *config) {
    this->set_from_config(config);
}

Navestidlo::Navestidlo() {
}

void Navestidlo::set_from_config(const Navestidlo_pin_config *config) {
    this->zluta_up.set_pin(config->zluta_up);
    this->zelena.set_pin(config->zelena);
    this->cervena.set_pin(config->cervena);
    this->bila.set_pin(config->bila);
    this->zluta_down.set_pin(config->zluta_down);
    this->zeleny_prouzek_up.set_pin(config->zeleny_prouzek_up);
    this->zluty_prouzek.set_pin(config->zluty_prouzek);
    this->zeleny_prouzek_down.set_pin(config->zeleny_prouzek_down);
    this->modra.set_pin(config->modra);

    if (config->cervena != -1) this->permissive_pin = config->cervena;
    if (config->modra != -1) this->permissive_pin = config->modra;
    if (this->permissive_pin == -1) this->permissive_pin = config->zluta_up; // v pripade predvesti

}

void Navestidlo::clear_pins() {
    this->zluta_up.set_hodnota(LED_OFF);
    this->zelena.set_hodnota(LED_OFF);
    this->cervena.set_hodnota(LED_OFF);
    this->bila.set_hodnota(LED_OFF);
    this->zluta_down.set_hodnota(LED_OFF);
    this->zeleny_prouzek_up.set_hodnota(LED_OFF);
    this->zluty_prouzek.set_hodnota(LED_OFF);
    this->zeleny_prouzek_down.set_hodnota(LED_OFF);
    this->modra.set_hodnota(LED_OFF);
}

void Navestidlo::fillBuffer(int* buffer, int length) {
    if (length != COUNT_LEDS) return;

    this->zluta_up.fill_buffer(buffer, length);
    this->zelena.fill_buffer(buffer, length);
    this->cervena.fill_buffer(buffer, length);
    this->bila.fill_buffer(buffer, length);
    this->zluta_down.fill_buffer(buffer, length);
    this->zeleny_prouzek_up.fill_buffer(buffer, length);
    this->zluty_prouzek.fill_buffer(buffer, length);
    this->zeleny_prouzek_down.fill_buffer(buffer, length);
    this->modra.fill_buffer(buffer, length);
}

void Navestidlo::setZkracenaBrzda() {
    this->bila.set_hodnota(LED_ON);
}

int Navestidlo::setZnak(int i2c_code) {
    auto new_znak = Znak();
    new_znak.from_i2c_code(i2c_code);

    this->clear_pins();

    int err = 0;
    switch (new_znak.get_current())
    {
    case Znak::STUJ:
        err = err + this->cervena.set_hodnota(LED_ON);
        err = err + this->modra.set_hodnota(LED_ON);
        break;
    case Znak::POSUN_DOVOLEN:
        err = err + this->bila.set_hodnota(LED_ON);
        break;
    case Znak::CTYRICET:
        err = err + this->zluta_down.set_hodnota(LED_ON);
        break;
    case Znak::SEDESAT:
        err = err + this->zluta_down.set_hodnota(LED_ON);
        err = err + this->zluty_prouzek.set_hodnota(LED_ON);
        break;
    case Znak::OSMDESAT:
        err = err + this->zluta_down.set_hodnota(LED_ON);
        err = err + this->zeleny_prouzek_up.set_hodnota(LED_ON);
        break;
    case Znak::STO:
        err = err + this->zluta_down.set_hodnota(LED_ON);
        err = err + this->zeleny_prouzek_up.set_hodnota(LED_ON);
        err = err + this->zeleny_prouzek_down.set_hodnota(LED_ON);
        break;
    case Znak::PRIVOLAVACI_NAVEST:
        err = err + this->bila.set_hodnota(LED_BLINK_SLOW);
        break;
    }

    switch (new_znak.get_next())
    {
    case Znak::VOLNO:
        err = err + this->zelena.set_hodnota(LED_ON);
        break;
    case Znak::STUJ:
        err = err + this->zluta_up.set_hodnota(LED_ON);
        break;
    case Znak::CTYRICET:
        err = err + this->zluta_up.set_hodnota(LED_BLINK_SLOW);
        break;
    case Znak::SEDESAT:
        err = err + this->zluta_up.set_hodnota(LED_BLINK_FAST);
        break;
    case Znak::OSMDESAT:
        err = err + this->zelena.set_hodnota(LED_BLINK_SLOW);
        break;
    case Znak::STO:
        err = err + this->zelena.set_hodnota(LED_BLINK_FAST);
        break;
    }
    return err;
}

int Navestidlo::get_permissive_pin() {
    return this->permissive_pin;
}