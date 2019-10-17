#include "loader.h"
typedef struct Rom Rom;

struct Rom {
    char *name;
    char *path;
    size_t romsize;
    uint8_t *data;
    int isrom;
    char *fw_dir;
    char *fw_file;

    target_phys_addr_t addr;
    QTAILQ_ENTRY(Rom) next;
};
static QTAILQ_HEAD(, Rom) roms = QTAILQ_HEAD_INITIALIZER(roms);

static Rom *find_rom(target_phys_addr_t addr) {
    Rom *rom;

    QTAILQ_FOREACH (rom, &roms, next) {
        if (rom->fw_file) {
            continue;
        }
        if (rom->addr > addr) {
            continue;
        }
        if (rom->addr + rom->romsize < addr) {
            continue;
        }
        return rom;
    }
    return NULL;
}
void *rom_ptr(target_phys_addr_t addr) {
    Rom *rom;

    rom = find_rom(addr);
    if (!rom || !rom->data)
        return NULL;
    return rom->data + (addr - rom->addr);
}
