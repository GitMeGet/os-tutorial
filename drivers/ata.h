#ifndef ATA_H
#define ATA_H

#include <stdint.h>

#define ATA_REG_IO_BASE         0x1F0
#define ATA_REG_DATA_RW         (ATA_REG_IO_BASE + 0)
#define ATA_REG_ERR_R           (ATA_REG_IO_BASE + 1)
#define ATA_REG_FEAT_W          (ATA_REG_IO_BASE + 1)
#define ATA_REG_SECT_CNT_RW     (ATA_REG_IO_BASE + 2)
#define ATA_REG_LBA_LO_RW       (ATA_REG_IO_BASE + 3)
#define ATA_REG_LBA_MID_RW      (ATA_REG_IO_BASE + 4)
#define ATA_REG_LBA_HI_RW       (ATA_REG_IO_BASE + 5)
#define ATA_REG_DRIVE_RW        (ATA_REG_IO_BASE + 6)
#define ATA_REG_STATUS_R        (ATA_REG_IO_BASE + 7)
#define ATA_REG_CMD_W           (ATA_REG_IO_BASE + 7)
#define ATA_REG_CTL_BASE        0x3F6
#define ATA_REG_ALT_STATUS_R    (ATA_REG_CTL_BASE + 0)
#define ATA_REG_DEV_CTL_W       (ATA_REG_CTL_BASE + 0)
#define ATA_REG_DRIVE_ADDR_R    (ATA_REG_CTL_BASE + 1)

#define ATA_REG_DRIVE_MASTER    0xA0
#define ATA_REG_DRIVE_SLAVE     0xB0
#define ATA_REG_DRIVE_LBA_MODE  0x40

#define ATA_REG_CMD_READ        0x20
#define ATA_REG_CMD_IDENTIFY    0xEC

#define ATA_REG_STATUS_BSY      0x80
#define ATA_REG_STATUS_RDY      0x40
#define ATA_REG_STATUS_DF       0x20
#define ATA_REG_STATUS_DRQ      0x08
#define ATA_REG_STATUS_ERR      0x01

#define ATA_SECTOR_BYTES        512

int ata_identify(void);
int ata_read_sectors(uint16_t* dest, uint32_t LBA, uint8_t sector_count);

#endif /* ATA_H */
