#include "ata.h"
#include "drivers/ports.h"
#include "drivers/timer.h"
#include "drivers/vga.h"

static
void ata_wait_not_bsy(void) {
    /* poll Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears */
    while ((port_byte_in(ATA_REG_ALT_STATUS_R) & ATA_REG_STATUS_BSY) != 0);
}

static
void ata_wait_drq(void) {
    uint8_t reg_val = 0;
    while (reg_val == 0) {
        reg_val = port_byte_in(ATA_REG_ALT_STATUS_R) &
                  (ATA_REG_STATUS_DRQ | ATA_REG_STATUS_ERR | ATA_REG_STATUS_DF);
    }
}

static
int ata_poll(void) {
    /* wait for BSY flag to be set */
    timer_sleep_ms(5);

    ata_wait_not_bsy();

    uint8_t status = port_byte_in(ATA_REG_ALT_STATUS_R);
    if (status & ATA_REG_STATUS_ERR) {
        return -1;
    } else if (status & ATA_REG_STATUS_DF) {
        return -2;
    } else if (!(status & ATA_REG_STATUS_DRQ)) {
        return -3;
    } else {
        return 0;
    }
}

int ata_identify(void) {
    uint16_t data;

    port_byte_out(ATA_REG_DRIVE_RW, ATA_REG_DRIVE_MASTER);
    port_byte_out(ATA_REG_SECT_CNT_RW, 0);
    port_byte_out(ATA_REG_LBA_LO_RW, 0);
    port_byte_out(ATA_REG_LBA_MID_RW, 0);
    port_byte_out(ATA_REG_LBA_HI_RW, 0);
    port_byte_out(ATA_REG_CMD_W, ATA_REG_CMD_IDENTIFY);

    if (port_byte_in(ATA_REG_STATUS_R) == 0) {
        return -1;
    };

    ata_wait_not_bsy();

    /* Because of some ATAPI drives that do not follow spec,
     * check the LBAmid and LBAhi ports (0x1F4 and 0x1F5) if they are non-zero.
     * If so, the drive is not ATA, and you should stop polling. */
    if (port_byte_in(ATA_REG_LBA_MID_RW) != 0 ||
         port_byte_in(ATA_REG_LBA_HI_RW) != 0) {
        return -4;
    }

    /* poll Status port until bit 3 (DRQ, value = 8) sets,
     * or until bit 0 (ERR, value = 1) sets. */
    ata_wait_drq();

    /* if ERR is clear, the data is ready to read from the Data port (0x1F0).
     * Read 256 16-bit values, and store them. */
    if ((port_byte_in(ATA_REG_STATUS_R) & ATA_REG_STATUS_ERR) != 0) {
        return -1;
    }

    for (int i = 0; i < 256; i++) {
        data = port_word_in(ATA_REG_DATA_RW);
        printf("%d: %d\n", i, data);
    }

    return 0;
}

int ata_read_sectors(uint16_t* dest, uint32_t LBA, uint8_t sector_count)
{
    int ret_val;

    ata_wait_not_bsy();

    /* select drive */
    port_byte_out(ATA_REG_DRIVE_RW, ATA_REG_DRIVE_MASTER |
                                    ATA_REG_DRIVE_LBA_MODE |
                                    ((LBA >> 24) & 0x0F));

    /* disable interrupts after drive selected */
    port_byte_out(ATA_REG_DEV_CTL_W, 2);

    port_byte_out(ATA_REG_SECT_CNT_RW, sector_count);
    port_byte_out(ATA_REG_LBA_LO_RW,  (uint8_t) LBA);
    port_byte_out(ATA_REG_LBA_MID_RW, (uint8_t) (LBA >> 8));
    port_byte_out(ATA_REG_LBA_HI_RW,  (uint8_t) (LBA >> 16));
    port_byte_out(ATA_REG_CMD_W, ATA_REG_CMD_READ);

    for (int i = 0; i < sector_count; i++) {

        if (ret_val = ata_poll()) {
            vga_print("ata_poll() err\n");
            return ret_val;
        }

        for (int j = 0; j < ATA_SECTOR_BYTES; j++) {
            dest[j] = port_word_in(ATA_REG_DATA_RW);
        }

        dest += ATA_SECTOR_BYTES;
    }
}

int ata_write_sectors(uint16_t* src, uint32_t LBA, uint8_t sector_count)
{
    int ret_val;

    ata_wait_not_bsy();

    /* select drive */
    port_byte_out(ATA_REG_DRIVE_RW, ATA_REG_DRIVE_MASTER |
                                    ATA_REG_DRIVE_LBA_MODE |
                                    ((LBA >> 24) & 0x0F));

    /* disable interrupts after drive selected */
    port_byte_out(ATA_REG_DEV_CTL_W, 2);

    port_byte_out(ATA_REG_SECT_CNT_RW, sector_count);
    port_byte_out(ATA_REG_LBA_LO_RW,  (uint8_t) LBA);
    port_byte_out(ATA_REG_LBA_MID_RW, (uint8_t) (LBA >> 8));
    port_byte_out(ATA_REG_LBA_HI_RW,  (uint8_t) (LBA >> 16));
    port_byte_out(ATA_REG_CMD_W, ATA_REG_CMD_WRITE);

    for (int i = 0; i < sector_count; i++) {

        if (ret_val = ata_poll()) {
            vga_print("ata_poll() err\n");
            return ret_val;
        }

        for (int j = 0; j < ATA_SECTOR_BYTES; j++) {
            port_word_out(ATA_REG_DATA_RW, src[j]);
        }

        src += ATA_SECTOR_BYTES;
    }
}

