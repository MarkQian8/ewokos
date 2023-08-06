#ifndef SD_H
#define SD_H

#include <sys/ewokdef.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t (*sd_init_func)(void);
typedef int32_t (*sd_read_sector_func)(int32_t sector, void* buf);
typedef int32_t (*sd_write_sector_func)(int32_t sector, const void* buf);

int32_t sd_read_sector(int32_t sector, void* buf, uint32_t cache);
int32_t sd_write_sector(int32_t sector, const void* buf); 
int32_t sd_read(int32_t block, void* buf, uint32_t cache); 
int32_t sd_write(int32_t block, const void* buf); 
int32_t sd_init(sd_init_func init, sd_read_sector_func rd, sd_write_sector_func wr);
int32_t sd_quit(void);
int32_t sd_set_buffer(uint32_t sector_num);

#define SECTOR_SIZE 512

#ifdef __cplusplus
}
#endif

#endif
