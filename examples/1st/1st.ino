//===============================
// 1st
//===============================
#include "tinyCommand.hpp"
static tinyCommand tC(Serial);
// static SerialCmd tC(Serial2);
// static SerialCmd tC;

//===============================
// cmd HELP/?
//===============================
int16_t help(int argc, char **argv) {
  static bool setuped = false;

  static int stat = 1;
  if (!setuped) {
    setuped = true;
  } else {
    Serial.printf("Available Commands:\n"
                  "- ? / help    : Display usage information.\n"
                  "- led1 [0/1]  : Toggle LED1 on/off.\n"
                  "- led2 [0/1]  : Toggle LED2 on/off.\n"
                  "- reset       : Reboot the system.\n"
                 );
  }
  return 0;
}

//===============================
// cmd led1
// This content has been verified on SPRESENSE.
// Please modify it for your CPU and proceed with the implementation.
//===============================
int16_t led1(int argc, char **argv) {
  static bool setuped = false;
  static int stat = 1;
  if (argc > 1) stat = atoi(argv[1]);

  if (!setuped) {
    pinMode(LED1, OUTPUT);
    setuped = true;
  }
  digitalWrite(LED1, stat);
  Serial.printf("led : %d\n", stat);
  stat = !stat;
  return 0;
}

//===============================
// cmd led2
// This content has been verified on SPRESENSE.
// Please modify it for your CPU and proceed with the implementation.
//===============================
int16_t led2(int argc, char **argv) {
  static bool setuped = false;
  static int stat = 1;

  if (argc > 1) stat = atoi(argv[1]);
  if (!setuped) {
    pinMode(LED2, OUTPUT);
    setuped = true;
  }
  digitalWrite(LED2, stat);
  Serial.printf("led : %d\n", stat);
  stat = !stat;
  return 0;
}

//===============================
// cmd reset
// This content has been verified on SPRESENSE.
// Please modify it for your CPU and proceed with the implementation.
//===============================
#include <LowPower.h>
int16_t reset(int argc, char **argv) {
  static bool setuped = false;

  if (!setuped) {
    setuped = true;
  } else {
    LowPower.begin();
    LowPower.reboot();
  }
  return 0;
}

//===============================
// setup
//===============================
void setup(void) {
  Serial.begin( 115200 ); delay(500);
  tC.begin();
  tC.setCmd("help", help);
  tC.setCmd("?", help);
  tC.setCmd("led1", led1);
  tC.setCmd("led2", led2);
  tC.setCmd("reset", reset);
}

//===============================
// loop
//===============================
void loop(void) {
  tC.scan();
}
