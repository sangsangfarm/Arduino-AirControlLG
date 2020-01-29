/**
 * @file AirControl_LG.cpp
 * @brief LG 에어콘 제어
 * @date 2019-09-18
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#include "AirControl_LG.h"

/**
 * @fn AirControl_LG::AirControl_LG(void)
 * @brief LG 에어콘 제어 클래스 생성자
 * @return LG 에어콘 제어 클래스
 * @date 2019-09-18
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
AirControl_LG::AirControl_LG(void) {}

/**
 * @fn AirControl_LG::AirControl_LG(uint16_t pin)
 * @brief LG 에어콘 제어 클래스 생성자
 * @return LG 에어콘 제어 클래스
 * @parm pin 에어콘 제어 핀
 * @date 2019-09-18
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
AirControl_LG::AirControl_LG(uint16_t pin) { setIRsendPin(pin); }

/**
 * @fn AirControl_LG::~AirControl_LG(void)
 * @brief LG 에어콘 제어 클래스 소멸자
 * @return LG 에어콘 제어 클래스
 * @date 2019-09-18
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
AirControl_LG::~AirControl_LG(void) {}
// Get IRSend.
IRsend AirControl_LG::getIRsend(void) {
  IRsend irsend(_irsendPin);
  irsend.begin();
  return irsend;
}
// Show air control status.
void AirControl_LG::showAirControlStatus(AirControlStatus air_control_status) {
  print(F("power : "));
  println(air_control_status.power);
  print(F("mode : "));
  println(air_control_status.mode);
  print(F("air_flow : "));
  println(air_control_status.flow);
  print(F("type : "));
  println(air_control_status.type);
  print(F("temperature : "));
  println(air_control_status.temperature);
  print(F("check_interval_second : "));
  println(air_control_status.check_interval_second);
  print(F("high_temperature : "));
  println(air_control_status.high_temperature);
  print(F("low_temperature : "));
  println(air_control_status.low_temperature);
  print(F("season : "));
  println(air_control_status.season);
  println();
}
// Show current air control status.
void AirControl_LG::showCurrentAirControlStatus(void) {
  println(F("Current Air Control Status"));
  showAirControlStatus(_current_air_control_status);
}
// Show last air control status.
void AirControl_LG::showLastAirControlStatus(void) {
  println(F("Last Air Control Status"));
  showAirControlStatus(_last_air_control_status);
}

AirControlStatus AirControl_LG::getCurrentAirControlStatus(void) {
  return _current_air_control_status;
}
AirControlStatus AirControl_LG::getLastAirControlStatus(void) {
  return _last_air_control_status;
}
// Set the IRsend Pin
void AirControl_LG::setIRsendPin(int pin) { _irsendPin = pin; }

// Set the Power   0 : off   1 : on
void AirControl_LG::setPower(unsigned int power) {
  _current_air_control_status.power = power;
}

// Set the Type   0 : TOWER   1 : WALL
void AirControl_LG::setType(unsigned int type) {
  _current_air_control_status.type = type;
}

// Set the Mode   0 : Cooling   1 : Heating
void AirControl_LG::setMode(unsigned int mode) {
  _current_air_control_status.mode = mode;
}

// Set Flow   0 : low   1 : mid   2 : high   3 : max
void AirControl_LG::setFlow(unsigned int flow) {
  _current_air_control_status.flow = flow;
}
void AirControl_LG::setTemperature(unsigned int temperature) {
  _current_air_control_status.temperature = temperature;
}
// Set the low temperature boundary
void AirControl_LG::setLowTemperature(int low_temperature) {
  _current_air_control_status.low_temperature = low_temperature;
}
// Set the high temperature boundary
void AirControl_LG::setHighTemperature(int high_temperature) {
  _current_air_control_status.high_temperature = high_temperature;
}

// Set the low temperature in emergency
void AirControl_LG::setEmergencyLowTemperature(int emergency_low_temperature) {
  _current_air_control_status.emergency_low_temperature =
      emergency_low_temperature;
}

// Set the high temperature in emergency
void AirControl_LG::setEmergencyHighTemperature(
    int emergency_high_temperature) {
  _current_air_control_status.emergency_high_temperature =
      emergency_high_temperature;
}

// Set the default cool temperature
void AirControl_LG::setDefaultCoolTemperature(
    unsigned int default_cool_temperature) {
  _current_air_control_status.default_cool_temperature =
      default_cool_temperature;
}

// Set the default heat temperature
void AirControl_LG::setDefaultHeatTemperature(
    unsigned int default_heat_temperature) {
  _current_air_control_status.default_heat_temperature =
      default_heat_temperature;
}
// Set the check intraval seconds
void AirControl_LG::setCheckIntervalSecond(unsigned int check_interval_second) {
  _current_air_control_status.check_interval_second = check_interval_second;
}
// Set the season
void AirControl_LG::setSeason(unsigned int season) {
  _current_air_control_status.season = season;
}
void AirControl_LG::save() {
  memcpy(&_last_air_control_status, &_current_air_control_status,
         sizeof(AirControlStatus));
}

// Send the IR code
void AirControl_LG::sendCode(uint32_t code) {
  print(F("code to send : "));
  print(code, BIN);
  print(F(" : "));
  println(code, HEX);
  getIRsend().sendLG(code, 28, 2);
}
// Control air conditioner.
void AirControl_LG::control(void) {
  unsigned int ac_msbits1 = 8;
  unsigned int ac_msbits2 = 8;
  unsigned int ac_msbits3 = 0;
  unsigned int ac_msbits4;
  if (_current_air_control_status.mode == HEAT_MODE)
    ac_msbits4 = 4;  // heating
  else
    ac_msbits4 = 0;
  unsigned int ac_msbits5 = (_current_air_control_status.temperature < 15)
                                ? 0
                                : _current_air_control_status.temperature - 15;
  unsigned int ac_msbits6 = 0;

  if (_current_air_control_status.flow <= AIR_FLOW_MAX) {
    if (_current_air_control_status.type == TOWER_TYPE)
      ac_msbits6 = flow_tower[_current_air_control_status.flow];
    else
      ac_msbits6 = flow_wall[_current_air_control_status.flow];
  }
  unsigned int ac_msbits7 =
      (ac_msbits3 + ac_msbits4 + ac_msbits5 + ac_msbits6) & B00001111;
  _code = ac_msbits1 << 4;
  _code = (_code + ac_msbits2) << 4;
  _code = (_code + ac_msbits3) << 4;
  _code = (_code + ac_msbits4) << 4;
  _code = (_code + ac_msbits5) << 4;
  _code = (_code + ac_msbits6) << 4;
  _code = (_code + ac_msbits7);
  sendCode(_code);
}
// Control air conditioner temperature.
void AirControl_LG::controlTemperature(unsigned int temperature) {
  setTemperature(temperature);
  control();
}
// Control air conditioner flow.
void AirControl_LG::controlFlow(unsigned int flow) {
  setFlow(flow);
  control();
}
// Control air conditioner mode.
void AirControl_LG::controlMode(unsigned int mode) {
  setMode(mode);
  control();
}
// Control air conditioner mode.
void AirControl_LG::controlAll(unsigned int temperature, unsigned int air_flow,
                               unsigned int mode) {
  setMode(mode);
  setTemperature(temperature);
  setFlow(air_flow);
  setPower(POWER_ON);
  control();
}

// Power on air conditioner.
void AirControl_LG::powerOn(void) {
  controlAll(_current_air_control_status.temperature,
             _current_air_control_status.flow,
             _current_air_control_status.mode);
}
// Power off air conditioner.
void AirControl_LG::powerDown(void) {
  setPower(POWER_OFF);
  _code = 0x88C0051;
  sendCode(_code);
}
// Get air conditioner power status.
unsigned int AirControl_LG::getPower(void) {
  return _current_air_control_status.power;
}
// Get air conditioner type.
unsigned int AirControl_LG::getType(void) {
  return _current_air_control_status.type;
}
// Get air conditioner mode.
unsigned int AirControl_LG::getMode(void) {
  return _current_air_control_status.mode;
}
// Get air conditioner flow.
unsigned int AirControl_LG::getFlow(void) {
  return _current_air_control_status.flow;
}
// Get air conditioner temperature.
unsigned int AirControl_LG::getTemperature(void) {
  return _current_air_control_status.temperature;
}
// Get the season
unsigned int AirControl_LG::getSeason(void) {
  return _current_air_control_status.season;
}

AirControl_LG air_control;