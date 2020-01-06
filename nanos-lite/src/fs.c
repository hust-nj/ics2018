#include "fs.h"

extern size_t serial_write(const void *buf, size_t offset, size_t len);


typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0,invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

size_t fs_write(int fd, const void *buf, size_t len) {
	// size_t fs_size = fs_filesz(fd);
  printf("fs_write filename:%s\n", file_table[fd].name);
	switch(fd) {
		case FD_STDIN: break;
		case FD_STDOUT:
		case FD_STDERR:
			file_table[fd].write(buf, 0, len);
			break;
		default:
      panic("cannot reach here\n");
			break;
	}
	return len;
}

