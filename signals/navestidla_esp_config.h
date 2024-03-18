#ifndef NAVESTIDLA_ESP_CONFIG_H
#define NAVESTIDLA_ESP_CONFIG_H


#include "navestidlo.h"
// makra pro piny pro jednotlive konfigurace navestidel

// --------
// pozice 1
// --------
const Navestidlo_pin_config n11111_111_pozice_1 = {
    .zluta_up = 16,
    .zelena = 14,
    .cervena = 13,
    .bila = 15,
    .zluta_down = 17,
    .zeleny_prouzek_up = 0,
    .zluty_prouzek = 1,
    .zeleny_prouzek_down = 8,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config n11111_110_pozice_1 = {
    .zluta_up = 16,
    .zelena = 14,
    .cervena = 13,
    .bila = 15,
    .zluta_down = 17,
    .zeleny_prouzek_up = 0,
    .zluty_prouzek = 1,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config n11111_010_pozice_1 = {
    .zluta_up = 16,
    .zelena = 14,
    .cervena = 13,
    .bila = 15,
    .zluta_down = 17,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = 0,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config n01111_110_pozice_1 = {
    .zluta_up = UNUSED_PIN,
    .zelena = 14,
    .cervena = 13,
    .bila = 15,
    .zluta_down = 17,
    .zeleny_prouzek_up = 0,
    .zluty_prouzek = 1,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config n11111_000_pozice_1 = {
    .zluta_up = 16,
    .zelena = 14,
    .cervena = 13,
    .bila = 15,
    .zluta_down = 17,
    .zeleny_prouzek_up = 0,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

// --------
// pozice 2
// --------

const Navestidlo_pin_config n11111_010_pozice_2 = {
    .zluta_up = 5,
    .zelena = 3,
    .cervena = 2,
    .bila = 4,
    .zluta_down = 6,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = 7,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config n01111_101_pozice_2 = {
    .zluta_up = UNUSED_PIN,
    .zelena = 3,
    .cervena = 2,
    .bila = 4,
    .zluta_down = 6,
    .zeleny_prouzek_up = 5,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = 7,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config n01110_000_pozice_2A = {
    .zluta_up = UNUSED_PIN,
    .zelena = 3,
    .cervena = 2,
    .bila = 4,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config n01101_000_pozice_2A = {
    .zluta_up = UNUSED_PIN,
    .zelena = 3,
    .cervena = 2,
    .bila = UNUSED_PIN,
    .zluta_down = 4,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config predvest_pozice_2A = {
    .zluta_up = 2,
    .zelena = 3,
    .cervena = UNUSED_PIN, // abych ukazal vystrahu pri inicializaci
    .bila = UNUSED_PIN,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config predvest_pozice_2B = {
    .zluta_up = 5,
    .zelena = 6,
    .cervena = UNUSED_PIN,
    .bila = UNUSED_PIN,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config trpaslik_pozice_2B = {
    .zluta_up = UNUSED_PIN,
    .zelena = UNUSED_PIN,
    .cervena = UNUSED_PIN,
    .bila = 5,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = 6
};

// --------
// pozice 3
// --------

const Navestidlo_pin_config n01111_010_pozice_3 = {
    .zluta_up = UNUSED_PIN,
    .zelena = 9,
    .cervena = 8, // abych ukazal vystrahu pri inicializaci
    .bila = 10,
    .zluta_down = 12,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = 11,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config predvest_pozice_3A = {
    .zluta_up = 8,
    .zelena = 9,
    .cervena = UNUSED_PIN, // abych ukazal vystrahu pri inicializaci
    .bila = UNUSED_PIN,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config trpaslik_pozice_3A = {
    .zluta_up = UNUSED_PIN,
    .zelena = UNUSED_PIN,
    .cervena = UNUSED_PIN,
    .bila = 8,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = 9
};

const Navestidlo_pin_config n01101_000_pozice_3A = {
    .zluta_up = UNUSED_PIN,
    .zelena = 9,
    .cervena = 8,
    .bila = UNUSED_PIN,
    .zluta_down = 10,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

const Navestidlo_pin_config trpaslik_pozice_3B = {
    .zluta_up = UNUSED_PIN,
    .zelena = UNUSED_PIN,
    .cervena = UNUSED_PIN,
    .bila = 11,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = 12
};

const Navestidlo_pin_config n01100_000_pozice_3B = {
    .zluta_up = UNUSED_PIN,
    .zelena = 12,
    .cervena = 11,
    .bila = UNUSED_PIN,
    .zluta_down = UNUSED_PIN,
    .zeleny_prouzek_up = UNUSED_PIN,
    .zluty_prouzek = UNUSED_PIN,
    .zeleny_prouzek_down = UNUSED_PIN,
    .modra = UNUSED_PIN
};

#endif