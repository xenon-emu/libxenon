#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <xenos/xenos.h>
#include <diskio/ata.h>
#include <console/console.h>
#include <diskio/disc_io.h>
#include <sys/iosupport.h>
#include <usb/usbmain.h>
#include <ppc/timebase.h>
#include <xenon_nand/xenon_sfcx.h>
#include <xenon_soc/xenon_power.h>
#include <xenon_soc/xenon_secotp.h>
#include <elf/elf.h>

#include <libfat/fat.h>

int bdev_enum(int handle, const char **name);

// https://github.com/Free60Project/tools/blob/master/arnezamidump/dump32.c

#define MEM_1BL_LOC 0x8000020000000000ULL
#define MEM_1BL_SZ 0x8000

#define MEM_NAND_LOC 0x80000200C8000000ULL
#define MEM_NAND_SZ 0x1000000

int write_to_file(const char *path, u8* data, u64 size, const char *mode) {
  FILE *file = fopen(path, mode);
  if (!file) {
    printf("Failed to open %s!\n", path);
    return -1;
  }
  fwrite(data, size, 1, file);
  fclose(file);
  return 0;
}

int dump_1bl(const char *path) {
  char buf[256];
  strcpy(buf, path);
  strcat(buf, "1bl.bin");
  printf("Dumping 1bl.bin to %s\n", buf);
  u8 *onebl = (u8*)malloc(MEM_1BL_SZ);
  if (!onebl) {
    printf("Failed to allocate memory!\n");
    return -1;
  }
  printf("Alloc'd\n");
  u8 *memory = (u8*)MEM_1BL_LOC;
  memcpy(onebl, memory, MEM_1BL_SZ);
  printf("OneBL memory copy happened\n");
  printf("Read test: 0x%llX | 0x%llX\n", (u64)onebl[0], (u64)onebl[1]);
  FILE *file = fopen(buf, "wb");
  if (!file) {
    printf("Failed to open %s!\n", path);
    return -1;
  }
  //int status = write_to_file(buf, onebl, MEM_1BL_SZ, "wb");
  fwrite(onebl, 1, 10, file);
  printf("Wrote to file!\n");
  fclose(file);
  free(onebl);
  return 0;
}

u64 get_nand_size() {
  int size = sfc.size_bytes_phys;
  if((size == (RAW_NAND_64 * 4)) || (size == (RAW_NAND_64 * 8))) {
    if (size == (RAW_NAND_64 * 4))
      size = 256;
    else
      size = 512;
    size = RAW_NAND_64;
  }
  return size;
}

int dump_nand(const char *path) {
  char buf[256];
  strcpy(buf, path);
  strcat(buf, "nand.bin");
  u64 nand_size = get_nand_size();
  FILE *file = fopen(buf, "wb");
  if (!file) {
    printf("Failed to open %s!\n", path);
    return -1;
  }
  int status = rawflash_readImage(nand_size, file);
  fclose(file);
  return status;
}

int dump_fuses(const char *path) {
  char buf[256];
  strcpy(buf, path);
  strcat(buf, "fuses.txt");
  printf("Dumping fuses.txt to %s\n", buf);
  char fuses[512];
  for (int i = 0; i < 12; ++i) {
    u64 line = xenon_secotp_read_line(i);
    char formatted[32];
    snprintf(formatted, sizeof(formatted), "fuseset %02d: %016llx", i, line);
    strcat(fuses, formatted);
  }
  return write_to_file(buf, (u8*)fuses, strlen(fuses), "w");
}

int fuses_written = 0;
int nand_written = 0;
int onebl_written = 0;
void dump_data(const char *path) {
  /*if (fuses_written != 1) {
    int status = dump_fuses(path);
    if (status == 0) {
      printf("Done.\n");
      fuses_written = 1;
    }
  }
  if (nand_written != 1) {
    int status = dump_nand(path);
    if (status == 0) {
      printf("Done.\n");
      nand_written = 1;
    }
  }*/
  if (onebl_written != 1) {
    int status = dump_1bl(path);
    if (status == 0) {
      printf("Done.\n");
      onebl_written = 1;
    }
  }
}

int main() {
  const char *s;
  char path[256];

  //xenos_init(VIDEO_MODE_AUTO);  
  //console_init();
  xenon_make_it_faster(XENON_SPEED_FULL);

  sfcx_init();

  usb_init();
  usb_do_poll();

  xenon_ata_init();
  xenon_atapi_init();

  fatInitDefault();

  usb_do_poll();

  int handle = -1;
  handle = bdev_enum(handle, &s);
  if (handle < 0) {
    printf("No available block devices!\n");
	  return 0;
  }

  strcpy(path, s);
  strcat(path, ":/");

  while (/*fuses_written == 0 && nand_written == 0 &&*/ onebl_written == 0) {
    usb_do_poll();
    dump_data(path);
  }

  return 0;
}