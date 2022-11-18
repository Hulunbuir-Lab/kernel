#include "ahci.h"

static HBA_MEM *abar;
static int portno = -1;

u8 AHCI_BASE[4096 * 75];

#define PORT (abar -> ports + portno)


void ahci_init(u64 ahci_base) {
    probe_port((HBA_MEM *) ahci_base);
}

/*
 Code Reference : OSDEV
 */
int read_port(u32 startl, u32 starth, u32 count,
        u64 buf)
{
    PORT->is = 0xffff; // Clear pending interrupt bits
    int slot = find_cmdslot(PORT);
    if (slot == -1)
        return -1;
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(PORT->clb );
    cmdheader += slot;
    cmdheader->cfl = sizeof(FIS_REG_H2D) / sizeof(u32); // Command FIS size
    cmdheader->w = 0; // Read from device
    cmdheader->c = 1; // Read from device
    cmdheader->p = 1; // Read from device
    cmdheader->prdtl = (u16) ((count - 1) >> 4) + 1; // PRDT entries count
    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba );
    int i = 0;
    for (i = 0; i < cmdheader->prdtl - 1; i++)
    {
        cmdtbl->prdt_entry[i].dba = (u32) (buf & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbau = (u32) ((buf << 32) & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbc = 8 * 1024 - 1; // 8K bytes
        cmdtbl->prdt_entry[i].i = 0;
        buf += 4 * 1024; // 4K words
        count -= 16; // 16 sectors
    }
    cmdtbl->prdt_entry[i].dba = (u32) (buf & 0xFFFFFFFF);
    cmdtbl->prdt_entry[i].dbau = (u32) ((buf << 32) & 0xFFFFFFFF);
    cmdtbl->prdt_entry[i].dbc = count << 9; // 512 bytes per sector
    cmdtbl->prdt_entry[i].i = 0;
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D*) (&cmdtbl->cfis);
    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c = 1; // Command
    cmdfis->command = ATA_CMD_READ_DMA_EX;
    cmdfis->lba0 = (u8) startl;
    cmdfis->lba1 = (u8) (startl >> 8);
    cmdfis->lba2 = (u8) (startl >> 16);
    cmdfis->device = 1 << 6; // LBA mode
    cmdfis->lba3 = (u8) (startl >> 24);
    cmdfis->lba4 = (u8) starth;
    cmdfis->lba5 = (u8) (starth >> 8);
    cmdfis->countl = count & 0xff;
    cmdfis->counth = count >> 8;
    
    int spin = 0;
    while ((PORT->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		printf("Port is hung\n");
		return -1;
	}
    PORT->ci = (1 << slot);
    while (1)
    {
        if ((PORT->ci & (1 << slot)) == 0)
            break;
        if (PORT->is & HBA_PxIS_TFES)
        { // Task file error
            printf("Read disk error\n");
            return -1;
        }
    }
    if (PORT->is & HBA_PxIS_TFES)
    {
        printf("Read disk error\n");
        return -1;
    }
    while (PORT->ci != 0)
    {
        printf("Read disk error\n");
    }
    return 0;
}
/*
 Code Reference : OSDEV
 */
int write_port(u32 startl, u32 starth, u32 count,
        u64 buf)
{
    PORT->is = 0xffff; // Clear pending interrupt bits
    int slot = find_cmdslot(PORT);
    if (slot == -1)
        return -1;
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(PORT->clb );
    cmdheader += slot;
    cmdheader->cfl = sizeof(FIS_REG_H2D) / sizeof(u32); // Command FIS size
    cmdheader->w = 1; // Read from device
    cmdheader->c = 1; // Read from device
    cmdheader->p = 1; // Read from device
    cmdheader->prdtl = (u16) ((count - 1) >> 4) + 1; // PRDT entries count
    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba );
    int i = 0;
    for (i = 0; i < cmdheader->prdtl - 1; i++)
    {
        cmdtbl->prdt_entry[i].dba = (u32) (buf & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbau = (u32) ((buf << 32) & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbc = 8 * 1024 - 1; // 8K bytes
        cmdtbl->prdt_entry[i].i = 0;
        buf += 4 * 1024; // 4K words
        count -= 16; // 16 sectors
    }
    cmdtbl->prdt_entry[i].dba = (u32) (buf & 0xFFFFFFFF);
    cmdtbl->prdt_entry[i].dbau = (u32) ((buf << 32) & 0xFFFFFFFF);
    cmdtbl->prdt_entry[i].dbc = count << 9; // 512 bytes per sector
    cmdtbl->prdt_entry[i].i = 0;
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D*) (&cmdtbl->cfis);
    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c = 1; // Command
    cmdfis->command = ATA_CMD_WRITE_DMA_EX;
    cmdfis->lba0 = (u8) startl;
    cmdfis->lba1 = (u8) (startl >> 8);
    cmdfis->lba2 = (u8) (startl >> 16);
    cmdfis->device = 1 << 6; // LBA mode
    cmdfis->lba3 = (u8) (startl >> 24);
    cmdfis->lba4 = (u8) starth;
    cmdfis->lba5 = (u8) (starth >> 8);
    cmdfis->countl = count & 0xff;
    cmdfis->counth = count >> 8;
    
    int spin = 0;
    while ((PORT->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		printf("Port is hung\n");
		return -1;
	}
    
    PORT->ci = (1 << slot);
    while (1)
    {
        // In some longer duration reads, it may be helpful to spin on the DPS bit
        // in the PxIS port field as well (1 << 5)
        if ((PORT->ci & (1 << slot)) == 0)
            break;
        if (PORT->is & HBA_PxIS_TFES)
        { // Task file error
            printf("Read disk error\n");
            return -1;
        }
    }
    if (PORT->is & HBA_PxIS_TFES)
    {
        printf("Read disk error\n");
        return -1;
    }
    while (PORT->ci != 0)
    {
        printf("Read disk error\n");
    }
    return 0;
}
// To setup command fing a free command list slot
int find_cmdslot()
{
    u32 slots = (PORT->sact | PORT->ci);
    int num_of_slots = (abar->cap & 0x0f00) >> 8; // Bit 8-12
    int i;
    for (i = 0; i < num_of_slots; i++)
    {
        if ((slots & 1) == 0)
        {
            //		printf("[slot=%d]", i);
            //	if(i==0)
            return i;
            //	break;
        }
        slots >>= 1;
    }
    printf("Cannot find free command list entry\n");
    return -1;
}
// Check device type
static int check_type(HBA_PORT * port)
{
    u32 ssts = port->ssts;
    u8 ipm = (ssts >> 8) & 0x0F;
    u8 det = ssts & 0x0F;
    //printf ("\n ipm %d det %d sig %d", ipm, det, PORT->sig);
    if (det != HBA_PORT_DET_PRESENT) // Check drive status
        return AHCI_DEV_NULL;
    if (ipm != HBA_PORT_IPM_ACTIVE)
        return AHCI_DEV_NULL;
    switch (port->sig)
    {
        case SATA_SIG_ATAPI:
            return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
            return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
            return AHCI_DEV_PM;
        default:
            return AHCI_DEV_SATA;
    }
    return 0;
}
/*
 Code Reference : OSDEV
 */
void rebase_port()
{
	stop_cmd(PORT);	// Stop command engine
 
	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	PORT->clb = AHCI_BASE + (portno<<10);
	PORT->clbu = 0;
	memset((void*)((PORT->clb) ) , 0, 1024);
 
	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	PORT->fb = AHCI_BASE + (32<<10) + (portno<<8);
	PORT->fbu = 0;
	memset((void*)((PORT->fb) ), 0, 256);
 
	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)((PORT->clb) );
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
		cmdheader[i].ctbau = 0;
		memset((void*)(cmdheader[i].ctba ) , 0, 256);
	}
 
	start_cmd(PORT);	// Start command engine
}
 
// Start command engine
void start_cmd()
{
	// Wait until CR (bit15) is cleared
	while (PORT->cmd & HBA_PxCMD_CR)
		;
 
	// Set FRE (bit4) and ST (bit0)
	PORT->cmd |= HBA_PxCMD_FRE;
	PORT->cmd |= HBA_PxCMD_ST; 
}
 
// Stop command engine
void stop_cmd()
{
	// Clear ST (bit0)
	PORT->cmd &= ~HBA_PxCMD_ST;
 
	// Clear FRE (bit4)
	PORT->cmd &= ~HBA_PxCMD_FRE;
 
	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (PORT->cmd & HBA_PxCMD_FR)
			continue;
		if (PORT->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}
 
}
/*
 Code Reference : OSDEV
 */
void probe_port(HBA_MEM *abar_temp)
{
    printf("Inside probe_port\n");
    // Search disk in impelemented ports
    u32 pi = abar_temp->pi;
    int i = 0;
    while (i < 32)
    {
        if (pi & 1)
        {
            int dt = check_type((HBA_PORT *) &abar_temp->ports[i]);
            if (dt == AHCI_DEV_SATA)
            {
                printf("\nSATA drive found at port %d\n", i);
                abar = abar_temp;
                portno = i;
                rebase_port();
                printf("DONE AHCI INITIALISATION :: PORT REBASE\n");
                return;
            }
            else if (dt == AHCI_DEV_SATAPI)
            {
                printf("\nSATAPI drive found at port %d\n", i);
            }
            else if (dt == AHCI_DEV_SEMB)
            {
                printf("\nSEMB drive found at port %d\n", i);
            }
            else if (dt == AHCI_DEV_PM)
            {
                printf("\nPM drive found at port %d\n", i);
            }
            else
            {
                printf("\nNo drive found at port %d\n", i);
            }
        }
        pi >>= 1;
        i++;
    }
    printf("probe_port complete\n");
}
