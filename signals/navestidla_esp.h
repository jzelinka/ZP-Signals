#ifndef NAVESTIDLA_ESP_H
#define NAVESTIDLA_ESP_H

#define TCAADDR 0x70 // bus multiplex chip address

#include "navestidlo_element.h"
#include <Wire.h>
#include <Arduino.h>
#include <PubSubClient.h>

#define MAX_NUM_ADDRESSES 20
bool send_live_bits(void *argument);

class Navestidla_esp {
    private:
        Navestidlo_element *known_navestidla;
        int COUNT_NAVESTIDLA;
        bool debug;
        bool addresses_used[MAX_NUM_ADDRESSES] = {false};
        bool deska_available[MAX_NUM_ADDRESSES] = {true};
        bool send_error[MAX_NUM_ADDRESSES] = {false};
        uint8_t bus_address;
        int buffer[COUNT_LEDS] = {LED_OFF};
        bool sent_initial_config = false;
        PubSubClient *mqtt_client = NULL;
        char *mqtt_topic = NULL;

    public:
        Navestidla_esp(Navestidlo_element *known, int count_navestidla, bool debug, uint8_t bus_address);

        void set_mqtt_client_for_errors(PubSubClient *mqtt_client, char *mqtt_topic);
        void throw_error(const char *message);

        void init_all_desks();

        void prepare_buffer(uint8_t desk_address);
        void send_buffer(uint8_t desk_address);
        void reset_buffer();

        void send_to_arduino_on_address(uint8_t desk_address);
        void select_my_bus();
        int navestidlo_to_table_pos(const char *navestidlo);
        void send_navestidlo_form_table_pos(int table_pos);

        void handle_navestidlo(const char *navestidlo, char *payload, unsigned int length);
        void print_navestidlo_info(int table_pos);
        bool get_debug();
        bool get_address_used(int address);
        bool get_deska_available(int address);
        void set_deska_available(int address, bool available);
}; // end of class

#endif