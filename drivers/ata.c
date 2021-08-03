#include "ata.h"
#include "ports.h"
#include "vga.h"

int ata_identify(void) {
    port_byte_out(ATA_REG_DRIVE_RW, ATA_REG_DRIVE_MASTER);
    port_byte_out(ATA_REG_SECT_CNT_RW, 0);
    port_byte_out(ATA_REG_LBA_LO_RW, 0);
    port_byte_out(ATA_REG_LBA_MID_RW, 0);
    port_byte_out(ATA_REG_LBA_HI_RW, 0);
    port_byte_out(ATA_REG_CMD_W, ATA_REG_CMD_IDENTIFY);

    if (port_byte_in(ATA_REG_STATUS_R) == 0) {
        return -1;
    };

    /* poll Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears */
    while ((port_byte_in(ATA_REG_STATUS_R) & ATA_REG_STATUS_BSY) != 0);

    /* Because of some ATAPI drives that do not follow spec,
     * check the LBAmid and LBAhi ports (0x1F4 and 0x1F5) if they are non-zero.
     * If so, the drive is not ATA, and you should stop polling. */
    if (port_byte_in(ATA_REG_LBA_MID_RW) != 0 ||
         port_byte_in(ATA_REG_LBA_HI_RW) != 0) {
        return -1;
    }

    /* poll Status port until bit 3 (DRQ, value = 8) sets,
     * or until bit 0 (ERR, value = 1) sets. */
    uint8_t reg_val = 0;
    while (reg_val == 0) {
        reg_val = port_byte_in(ATA_REG_ALT_STATUS_R) &
                  (ATA_REG_STATUS_DRQ | ATA_REG_STATUS_ERR);
    }

    /* if ERR is clear, the data is ready to read from the Data port (0x1F0).
     * Read 256 16-bit values, and store them. */
    if ((port_byte_in(ATA_REG_STATUS_R) & ATA_REG_STATUS_ERR) != 0) {
        return -1;
    }

    for (int i = 0; i < 256; i++) {
        vga_print_dec(port_word_in(ATA_REG_DATA_RW));
        vga_print("\n");
    }

    return 0;
}
