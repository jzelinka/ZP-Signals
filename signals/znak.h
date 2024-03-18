#ifndef ZNAK_H
#define ZNAK_H

#include <stdlib.h> 
// #include <Arduino.h>
#include <string.h>

class Znak
{
private:
    // describes what should be done now
    int current;
    // what should be on the next navestidlo
    int next;

    const static int count_mqtt_znak = 8;
    char mqtt_znaky[count_mqtt_znak][18] = {
      "stuj",
      "volno",
      "posunDovolen",
      "40",
      "60",
      "80",
      "100",
      "privolavaciNavest"
    };

public:
    enum casti_znaku_alias{
        STUJ,
        VOLNO,
        POSUN_DOVOLEN,
        CTYRICET,
        SEDESAT,
        OSMDESAT,
        STO,
        PRIVOLAVACI_NAVEST,
        NONE,
        LAST
    };
    inline Znak();
    inline void from_i2c_code(int code);
    inline void from_mqtt(const char* current_or_next, const char* payload, int length);
    inline void set_aktualni(const char* payload, int length);
    inline void set_opakovana(const char* payload, int length);
    inline int get_current();
    inline int get_next();
    inline int to_i2c_code();
    inline void set_current(int current);
    inline void set_next(int next);
    inline bool operator==(const Znak& rhs);
    inline int mqtt_znak_to_int(const char* mqtt_znak, int length);
};

bool Znak::operator==(const Znak& rhs) {
    return this->current == rhs.current && this->next == rhs.next;
}

int Znak::get_current() {
    return this->current;
}

int Znak::get_next() {
    return this->next;
}

void Znak::set_current(int current) {
    this->current = current;
}

void Znak::set_next(int next) {
    this->next = next;
}

Znak::Znak() {
    // makes "STUJ" the initial value of the navesitdlo
    this->current = STUJ;
    this->next = STUJ;
}

void Znak::from_i2c_code(int code) {
    this->current = div(code, LAST).rem; 
    this->next = div(code, LAST).quot;
}


void Znak::from_mqtt(const char* current_or_next, const char* payload, int length) {
    if (strcmp(current_or_next, "aktualni") == 0) {
        this->current = mqtt_znak_to_int(payload, length);
    } else if (strcmp(current_or_next, "opakovana") == 0) {
        this->next = mqtt_znak_to_int(payload, length);
    }
}

void Znak::set_aktualni(const char* payload, int length) {
    this->current = mqtt_znak_to_int(payload, length);
}

void Znak::set_opakovana(const char* payload, int length) {
    this->next = mqtt_znak_to_int(payload, length);
}

int Znak::to_i2c_code() {
    // STUJ, don't send the next
    if (this->current == STUJ) {
        return this->current + NONE * LAST;
    }
    
    // posunDovolen, don't sent the next
    if (this->current == POSUN_DOVOLEN) {
        return this->current + NONE * LAST;
    }

    return this->current + this->next * LAST;
}

int Znak::mqtt_znak_to_int(const char* mqtt_znak, int length) {
    for (int i = 0; i < this->count_mqtt_znak; i++) {
        if (strncmp(mqtt_znak, mqtt_znaky[i], length) == 0) {
            return i;
        }
    }

    return -1;
}

#endif