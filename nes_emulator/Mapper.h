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
    virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;
    // Transform PPU bus address into CHR ROM offset
    virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

    // IRQ handling
    virtual bool irqState();    // Added declaration
    virtual void irqClear();    // Added declaration

    // Mirroring handling
    virtual MIRROR mirror();    // Added declaration

    // Reset
    virtual void reset();       // Added declaration

    // Scanline
    virtual void scanline();    // Added declaration

protected:
    // These are stored locally as many of the mappers require this information
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};
