#ifndef AirControl_LG_H_
#define AirControl_LG_H_
#include <Arduino.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Debug
#ifndef AIR_CONTROL_LG_DEBUG_PORT
#define AIR_CONTROL_LG_DEBUG_PORT Serial
#endif

#ifdef AIR_CONTROL_LG_DEBUG
#define print(...) AIR_CONTROL_LG_DEBUG_PORT.print(__VA_ARGS__)
#define printf(...) AIR_CONTROL_LG_DEBUG_PORT.printf(__VA_ARGS__)
#define println(...) AIR_CONTROL_LG_DEBUG_PORT.println(__VA_ARGS__)

#else
#define print(...)
#define printf(...)
#define println(...)
#endif

#define TOWER_TYPE 0
#define WALL_TYPE 1

#define COOL_MODE 0
#define HEAT_MODE 1

#define POWER_OFF 0
#define POWER_ON 1

#define AIR_FLOW_LOW 0
#define AIR_FLOW_MID 1
#define AIR_FLOW_HIGH 2
#define AIR_FLOW_MAX 3

#define NONE 0
#define SUMMER 1
#define WINTER 2

typedef struct AirControlStatus {
  // POWER_OFF: 0, POWER_ON: 1
  unsigned int power;
  // COOL_MODE: 0, HEAT_MODE: 1
  unsigned int mode;
  // AIR_FLOW_LOW: 0, AIR_FLOW_MID: 1, AIR_FLOW_HIGH: 2, AIR_FLOW_MAX: 3
  unsigned int flow;
  // TOWER_TYPE: 0, WALL_TYPE: 1
  unsigned int type;
  // 18~30
  unsigned int temperature;
  int low_temperature;
  int high_temperature;
  int emergency_low_temperature;
  int emergency_high_temperature;
  // 18~30
  unsigned int default_cool_temperature;
  // 18~30
  unsigned int default_heat_temperature;
  unsigned int check_interval_second;
  // NONE: 0, SUMMER: 1, WINTER: 2
  unsigned int season;
} AirControlStatus;
class AirControl_LG {
 private:
  int _irsendPin;
  uint32_t _code;
  AirControlStatus _current_air_control_status = {
      999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999};
  AirControlStatus _last_air_control_status = {
      999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999};
  const uint8_t flow_tower[4] = {0, 2, 4, 6};
  const uint8_t flow_wall[4] = {0, 1, 2, 4};

  IRsend getIRsend();
  void sendCode(uint32_t code);
  void showAirControlStatus(AirControlStatus air_control_status);

 public:
  AirControl_LG(void);
  AirControl_LG(uint16_t pin);
  AirControl_LG(int pin, unsigned int power, unsigned int type,
                unsigned int mode, unsigned int flow);
  ~AirControl_LG();

  AirControlStatus getCurrentAirControlStatus();
  AirControlStatus getLastAirControlStatus();

  void showCurrentAirControlStatus();
  void showLastAirControlStatus();

  void setIRsendPin(int pin);

  void setPower(unsigned int power);
  void setType(unsigned int type);
  void setMode(unsigned int mode);
  void setFlow(unsigned int flow);
  void setTemperature(unsigned int temperature);
  void setLowTemperature(int low_temperature);
  void setHighTemperature(int high_temperature);
  void setEmergencyLowTemperature(int emergency_low_temperature);
  void setEmergencyHighTemperature(int emergency_high_temperature);
  void setDefaultCoolTemperature(unsigned int default_cool_temperature);
  void setDefaultHeatTemperature(unsigned int default_heat_temperature);
  void setCheckIntervalSecond(unsigned int check_interval_second);
  void setSeason(unsigned int season);

  void powerOn(void);
  void powerDown(void);
  void control(void);
  void controlTemperature(unsigned int temperature);
  void controlFlow(unsigned int flow);
  void controlMode(unsigned int mode);
  void controlAll(unsigned int temperature, unsigned int flow,
                  unsigned int mode);
  unsigned int getPower(void);
  unsigned int getType(void);
  unsigned int getMode(void);
  unsigned int getFlow(void);
  unsigned int getTemperature(void);
  unsigned int getSeason(void);
  void save();
};
extern AirControl_LG air_control;
#endif