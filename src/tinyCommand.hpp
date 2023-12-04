#ifndef __SERIALCMD_HPP__
#define __SERIALCMD_HPP__

#include <Arduino.h>

#define CMDARGC_MAX (10)
#define CMDNAME_MAX (16)
#define BUFCNT_MAX (255)

typedef struct {
  char* name;
  int16_t (*p)(int, char**);
} FUNC_T;

class tinyCommand {

public:

  tinyCommand(void) : _serialStream(Serial) {}

  tinyCommand(Stream& stream) : _serialStream(stream) {}

  ~tinyCommand(void) {
    for (int i = 0; i < _cmdCnt; i++) {
      delete[] _f[i].name;
    }
  }

  void begin(void) {}

  void setCmd(const char* name, int16_t (*cmd)(int, char**)) {
    if (_cmdCnt < CMDARGC_MAX) {
      _f[_cmdCnt].name = new char[CMDNAME_MAX];
      strncpy(_f[_cmdCnt].name, name, CMDNAME_MAX - 1);
      _f[_cmdCnt].name[CMDNAME_MAX - 1] = '\0';
      _f[_cmdCnt].p = cmd;
      _f[_cmdCnt].p(0, NULL);
      _cmdCnt++;
    }
  }

  int16_t findCmd(char *buf, char **argv) {
    int16_t idx = -1;
    char *token = strtok(buf, " \n");
    int argc = 0;
    while (token != NULL && argc < CMDARGC_MAX) {
      if (token[strlen(token) - 1] == '\r') {
        token[strlen(token) - 1] = '\0';
      }
      argv[argc++] = token;
      token = strtok(NULL, " \n");
    }
    argv[argc] = NULL;
    for (int i = 0; i < _cmdCnt; i++) {
      if (strncmp(argv[0], _f[i].name, strlen(_f[i].name)) == 0) {
        idx = i;
        break;
      }
    }
    return idx;
  }

  void scan(void) {
    static char buf[BUFCNT_MAX];
    static uint8_t bPos = 0;
    if (_serialStream.available() > 0) {
      char ch = _serialStream.read();
      if (ch == '\n') {
        buf[bPos] = '\0';
        char *argv[CMDARGC_MAX + 1] = {NULL};
        int16_t cmdIdx = findCmd(buf, argv);
        if (cmdIdx != -1) {
          int argc = 0;
          while (argv[argc] != NULL && argc < CMDARGC_MAX) {
            argc++;
          }
          _f[cmdIdx].p(argc, argv);
        } else {
          _serialStream.println("Invalid command");
        }
        memset(buf, 0, BUFCNT_MAX);
        bPos = 0;
      } else {
        buf[bPos++] = ch;
        if (bPos >= BUFCNT_MAX) {
          memset(buf, 0, BUFCNT_MAX);
          bPos = 0;
        }
      }
    }
  }

private:
  FUNC_T _f[CMDARGC_MAX];
  Stream& _serialStream;
  uint16_t _cmdCnt = 0;
};

#endif
