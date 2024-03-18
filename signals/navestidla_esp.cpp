#include "navestidla_esp.h"

// constructor
Navestidla_esp::Navestidla_esp(Navestidlo_element *known, int count_navestidla, bool debug, uint8_t bus_address) {
    known_navestidla = known;
    COUNT_NAVESTIDLA = count_navestidla;
    this->debug = debug;
    this->bus_address = bus_address;

    for (int i = 0; i < COUNT_NAVESTIDLA; i++) {
        int deska_idx = known_navestidla[i].get_deska();
        this->addresses_used[deska_idx] = true;
    }
}

void Navestidla_esp::set_mqtt_client_for_errors(PubSubClient *mqtt_client, char *mqtt_topic) {
    this->mqtt_client = mqtt_client;
    this->mqtt_topic = mqtt_topic;

    this->throw_error("Navestidla_esp initialized");
}

void Navestidla_esp::throw_error(const char *message) {
    if (this->mqtt_client == NULL) {
        Serial.println("MQTT client not set");
        return;
    }
    Serial.println(message);
    this->mqtt_client->publish(this->mqtt_topic, message);
}

void Navestidla_esp::init_all_desks() {
    for (int i = 0; i < MAX_NUM_ADDRESSES; i++) {
        if (this->addresses_used[i]) {
            this->send_to_arduino_on_address(i);
        }
    }

}

int Navestidla_esp::navestidlo_to_table_pos(const char *navestidlo)
{
    for (int i = 0; i < COUNT_NAVESTIDLA; i++)
    {
        if (strcmp(navestidlo, known_navestidla[i].get_name()) == 0)
        {
            return i;
        }
    }
    String error = "Unknown navestidlo ";
    error.concat(navestidlo);
    this->throw_error(error.c_str());
    // ALERT!!! Unknown navestidlo
    return -1;
}

void Navestidla_esp::print_navestidlo_info(int table_pos)
{
    Serial.print("Navestidlo: ");
    Serial.print(known_navestidla[table_pos].get_name());
    Serial.print(" deska: ");
    Serial.print(known_navestidla[table_pos].get_deska());
    auto tmp_znak = known_navestidla[table_pos].get_znak();
    Serial.print(" current: ");
    Serial.print(tmp_znak.get_current());
    Serial.print(" next: ");
    Serial.print(tmp_znak.get_next());
    Serial.print(" i2c_znak: ");
    Serial.println(tmp_znak.to_i2c_code());
}

void Navestidla_esp::prepare_buffer(uint8_t desk_address) {
    for (int i = 0; i < COUNT_NAVESTIDLA; i++) {
        if (known_navestidla[i].get_deska() == desk_address) {
            known_navestidla[i].fillBuffer(this->buffer, COUNT_LEDS);
        }
    }
}

void Navestidla_esp::send_buffer(uint8_t desk_address) {
    this->select_my_bus();
    Wire.beginTransmission(desk_address);
    for (int i = 0; i < COUNT_LEDS; i++) Wire.write(this->buffer[i]);
    Wire.endTransmission();
}

void Navestidla_esp::reset_buffer() {
    for (int i = 0; i < COUNT_LEDS; i++) this->buffer[i] = LED_OFF;
}

void Navestidla_esp::send_to_arduino_on_address(uint8_t desk_address) {
    this->reset_buffer();
    this->prepare_buffer(desk_address);
    this->send_buffer(desk_address);

    for (int i = 0; i < COUNT_LEDS; i++) {Serial.print(this->buffer[i]);}
    Serial.println();
}

void Navestidla_esp::send_navestidlo_form_table_pos(int table_pos)
{
    this->select_my_bus();
    int8_t desk_address = known_navestidla[table_pos].get_deska();

    this->send_to_arduino_on_address(desk_address);
}

void Navestidla_esp::handle_navestidlo(const char *navestidlo, char *payload, unsigned int length)
{
    // find navestidlo
    int table_pos = navestidlo_to_table_pos(navestidlo);

    if (table_pos == -1)
    {
        Serial.print("Unknown navestidlo ");
        Serial.println(navestidlo);
        return;
    }

    int err = known_navestidla[table_pos].update_znak(payload, length);
    
    if (err != 0)
    {
        payload[length] = '\0';
        Serial.print("Error updating navestidlo ");
        Serial.println(navestidlo);
        String error = "Error setting ";
        error.concat(payload);
        error.concat(" on ");
        error.concat(navestidlo);
        this->throw_error(error.c_str());
    }

    if (this->debug)
    {
        Serial.println("Navestidlo updated");
        print_navestidlo_info(table_pos);
    }
    send_navestidlo_form_table_pos(table_pos);
}

void Navestidla_esp::select_my_bus()
{
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << this->bus_address);
    Wire.endTransmission();
}

bool Navestidla_esp::get_debug()
{
    return this->debug;
}

bool Navestidla_esp::get_address_used(int address)
{
    return this->addresses_used[address];
}

bool Navestidla_esp::get_deska_available(int address)
{
    return this->deska_available[address];
}

void Navestidla_esp::set_deska_available(int address, bool available)
{
    if (this->deska_available[address] && !available) {
        this->send_error[address] = true;
    }

    if (this->send_error[address] && mqtt_client != NULL) {
        this->send_error[address] = false;
        String error = "Deska ";
        error.concat(address);
        error.concat(" is unavailable!");
        this->mqtt_client->publish(this->mqtt_topic, error.c_str());
    }

    this->deska_available[address] = available;

}

bool send_live_bits(void *argument)
{
    Navestidla_esp* navestidla_esp = (Navestidla_esp*) argument;

    if (navestidla_esp->get_debug()) {
        Serial.println("Sending live bit");
    }

    navestidla_esp->select_my_bus();

    for (int8_t desk_idx = 0; desk_idx < MAX_NUM_ADDRESSES; desk_idx++) {
        if (!(navestidla_esp->get_address_used(desk_idx))) {
            continue;
        }
        if (navestidla_esp->get_debug()) { 
            Serial.print("Sending live bit to address: ");
            Serial.println(desk_idx);
        }
        // navestidla_esp->send_to_arduino_on_address(desk_idx);

        Wire.beginTransmission(desk_idx);
        Wire.write(0);
        Wire.endTransmission();

        Wire.requestFrom(desk_idx, 1);
        // waiting for the data until the timeout expires
        unsigned long startTime = millis();
        unsigned long timeoutDuration = 50; // 1 second timeout
        while (Wire.available() < 1 && (millis() - startTime) < timeoutDuration) { }

        if (Wire.available() >= 1) {
            int receivedData = Wire.read();
            Serial.print("Deska availible: ");
            Serial.println(desk_idx);

            // sends data to deska on reconnection
            if (navestidla_esp->get_deska_available(desk_idx) == false) {
                navestidla_esp->send_to_arduino_on_address(desk_idx);
                navestidla_esp->set_deska_available(desk_idx, true);
                
            }

        } else {
            Serial.println("Timeout - No data received.");
            navestidla_esp->set_deska_available(desk_idx, false);
        }
    }

    return true;
}