#pragma once
#include <cstdint>

enum class MIRROR
{
    HORIZONTAL,
    VERTICAL,
    ONE_SCREEN_LO,
    ONE_SCREEN_HI,
    HARDWARE
};

class Mapper
{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();

public:
    // Transform CPU bus address into PRG ROM offset
    virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data) = 0; // Added data parameter
    // Transform PPU bus address into CHR ROM offset
    virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

    // IRQ handling
    virtual bool irqState();
    virtual void irqClear();

    // Mirroring handling
    virtual MIRROR mirror();

    // Reset
    virtual void reset();

    // Scanline
    virtual void scanline();

protected:
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};
