#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>
#define SCREEN_PORT 0x100
static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      uint32_t screen;
      screen = inl(SCREEN_PORT);
      info->width = screen >> 16;
      info->height = screen << 16 >> 16;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
	switch (reg) {
		case _DEVREG_VIDEO_FBCTL: {
			_FBCtlReg *ctl = (_FBCtlReg *)buf;
			int i;
			for(i = 0;i < ctl->h;i ++)
			{
			  memcpy(fb+(ctl->y+i)*screen_width()+ctl->x,ctl->pixels+i*ctl->w,ctl->w*4);
			}
			if (ctl->sync) {
			// do nothing, hardware syncs.
			}
			return sizeof(_FBCtlReg);
		}
	}
	return 0;
}

void vga_init() {
}
