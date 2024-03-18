#ifndef NAVESTIDLO_ELEMENT_H
#define NAVESTIDLO_ELEMENT_H

#include "znak.h"
#include "navestidlo.h"
#include "navestidla_esp_config.h"

#include <ArduinoJson.h>

#define JSON_BUFFER_SIZE 512

class Navestidlo_element {
    private:
        const char* name;
        int deska;
        Znak znak;
        bool already_got_first_part = false;
        Navestidlo navestidlo = Navestidlo();

    public:
        Navestidlo_element(const char* name, int deska, const Navestidlo_pin_config *config) {
            this->navestidlo.set_from_config(config);
            this->name = name;
            this->deska = deska;
            this->znak = Znak();
            this->navestidlo.setZnak(this->znak.to_i2c_code());
        }

        const char* get_name() {
            return this->name;
        }

        bool get_already_got_first_part() {
            return this->already_got_first_part;
        }

        void set_already_got_first_part() {
            this->already_got_first_part = true;
        }

        void reset_already_got_first_part() {
            this->already_got_first_part = false;
        }

        int get_deska() {
            return this->deska;
        }
        
        int update_znak(const char* payload, unsigned int length) {
            StaticJsonDocument<JSON_BUFFER_SIZE> parsed_json;
            DeserializationError err = deserializeJson(parsed_json, payload, length);
            if (err) {
                Serial.print("Error parsing JSON: ");
                Serial.println(err.c_str());
                return -1;
            }

            const char* aktualni_znak = "stuj";
            if (parsed_json.containsKey("aktualni")) {
                aktualni_znak = parsed_json["aktualni"];
            } else {
                Serial.println("Aktualni znak neni v JSONu");
            }

            const char* opakovany_znak = "stuj";
            if (parsed_json.containsKey("opakovana")) {
                opakovany_znak = parsed_json["opakovana"];
            } else {
                Serial.println("Opakovany znak neni v JSONu");
            }

            int length_of_aktualni = strlen(aktualni_znak);
            int length_of_opak = strlen(opakovany_znak);

            bool zkracena_brzda = false;
            if (parsed_json.containsKey("zkracena_brzda"))
            {
                zkracena_brzda = parsed_json["zkracena_brzda"];
            }

            if (length_of_aktualni == 0) {
                this->znak.set_aktualni("stuj", 4);
                Serial.println("Aktualni znak je prazdny");
            } else {
                this->znak.set_aktualni(aktualni_znak, length_of_aktualni);
            }

            if (length_of_opak == 0) {
                this->znak.set_opakovana("stuj", 4);
                Serial.println("Opakovany znak je prazdny.");
            } else {
                this->znak.set_opakovana(opakovany_znak, length_of_opak);
            }

            int err_int = this->navestidlo.setZnak(this->znak.to_i2c_code());
            if (zkracena_brzda) {
                this->navestidlo.setZkracenaBrzda();
            } 

            return err_int;
        }

        const Znak get_znak() {
            return this->znak;
        }

        void fillBuffer(int* buffer, int length) {
            if (length != COUNT_LEDS) return;

            this->navestidlo.fillBuffer(buffer, length);
        }
};

class Navestidlo_element_predvest : public Navestidlo_element {
    // to init the default value of the predvest
    public:
        Navestidlo_element_predvest(const char* name, int deska, const Navestidlo_pin_config *config) : Navestidlo_element(name, deska, config) {
            // TODO make this nicer
            const char* json_payload = "{\"aktualni\":\"volno\",\"opakovana\":\"stuj\"}";
            this->update_znak(json_payload, strlen(json_payload));
        }

};

#endif