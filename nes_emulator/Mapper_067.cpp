#include "Mapper_067.h"
#include "Mapper.h"


Mapper_067::Mapper_067(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks), prgBankSelect(0), chrBankSelect(0)
{
}

Mapper_067::~Mapper_067()
{
}

void Mapper_067::reset()
{
    prgBankSelect = 0;
    chrBankSelect = 0;
}

bool Mapper_067::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // Implement address mapping logic for CPU reads
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        // PRG Bank Switching logic
        mapped_addr = (prgBankSelect * 0x8000) + (addr & 0x7FFF);
        return true;
    }
    return false;
}

bool Mapper_067::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    // Implement address mapping logic for CPU writes
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        // Control bank switching via CPU write
        prgBankSelect = data & 0x07; // Select 8KB PRG bank
        chrBankSelect = (data >> 4) & 0x07; // Select 8KB CHR bank
        return true;
    }
    return false;
}

bool Mapper_067::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // Implement address mapping logic for PPU reads
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        // CHR Bank Switching logic
        mapped_addr = (chrBankSelect * 0x2000) + addr;
        return true;
    }
    return false;
}

bool Mapper_067::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        if (nCHRBanks == 0)
        {
            // Treat as RAM
            mapped_addr = addr;
            return true;
        }
    }
    return false;
}
