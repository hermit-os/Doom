#include "doomgeneric.h"
#include "doomkeys.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

extern int usleep(unsigned int usec);

extern ssize_t sys_read_keyboard(uint8_t *buffer, size_t size,
                                 bool nonblocking);

static uint32_t startTime = 0;

typedef struct {
  uint8_t *framebuffer;
  uint32_t width;
  uint32_t height;
  uint32_t bpp;
} FramebufferInfo;

static FramebufferInfo g_FramebufferInfo;

extern int sys_get_framebuffer_info(FramebufferInfo *info);

static unsigned char convertToDoomKey(unsigned char key) {
  switch (key) {
  case 0x11:
    return KEY_UPARROW;
  case 0x1E:
    return KEY_LEFTARROW;
  case 0x1F:
    return KEY_DOWNARROW;
  case 0x20:
    return KEY_RIGHTARROW;
  case 0x39:
    return KEY_USE;
  case 0x21:
    return KEY_FIRE;
  case 0x1C:
    return KEY_ENTER;
  case 0x01:
    return KEY_ESCAPE;
  case 0x1D:
    return KEY_RCTRL;
  case 0x2A:
  case 0x36:
    return KEY_RSHIFT;
  case 0x48:
    return KEY_UPARROW;
  case 0x50:
    return KEY_DOWNARROW;
  case 0x4B:
    return KEY_LEFTARROW;
  case 0x4D:
    return KEY_RIGHTARROW;
  case 0x2C:
    return 'y';
  case 0x31:
    return 'n';
  default:
    return 0;
  }
}

int DG_GetKey(int *pressed, unsigned char *doomKey) {
  uint8_t scancode;
  while (sys_read_keyboard(&scancode, 1, true) == 1) {
    unsigned char key = convertToDoomKey(scancode & 0x7F);
    if (key != 0) {
      *pressed = (scancode & 0x80) == 0 ? 1 : 0;
      *doomKey = key;
      return 1;
    } else {
      printf("Unknown scancode: 0x%02X\n", scancode);
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  doomgeneric_Create(argc, argv);

  while (1) {
    int start = DG_GetTicksMs();

    doomgeneric_Tick();
    int end = DG_GetTicksMs();
    int elapsed = end - start;
    if (elapsed < 28) {
      DG_SleepMs(28 - elapsed);
    }
  }

  return 0;
}

void DG_Init() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  startTime = (uint32_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
  sys_get_framebuffer_info(&g_FramebufferInfo);
}

void DG_DrawFrame() {
  uint32_t *framebuffer = (uint32_t *)g_FramebufferInfo.framebuffer;

  memcpy((void *)framebuffer, DG_ScreenBuffer,
         DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);
}

void DG_SleepMs(uint32_t ms) { usleep(ms * 1000); }

uint32_t DG_GetTicksMs() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  uint32_t current_time = (uint32_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
  return current_time - startTime;
}

void DG_SetWindowTitle(const char *title) {
  printf("DG_SetWindowTitle(%s)\n", title);
}
