#pragma once

#include <cstdint>
#include <array>
#include <memory> // Include for std::shared_ptr

#include "olc6502.h"
#include "olc2C02.h"
#include "olc2A03.h"
#include "Cartridge.h"

class Bus
{
public:
    Bus();
    ~Bus();

public: // Devices on Main Bus
    olc6502 cpu;                             // The 6502 derived processor
    olc2C02 ppu;                             // The 2C02 Picture Processing Unit
    olc2A03 apu;                             // The 2A03 Audio Processing Unit
    std::shared_ptr<Cartridge> cart;         // The Cartridge or "GamePak"
    uint8_t cpuRam[2048];                    // 2KB of RAM
    uint8_t controller[2];                   // Controller

public:
    void SetSampleFrequency(uint32_t sample_rate);
    double dAudioSample = 0.0;

private:
    double dAudioTime = 0.0;
    double dAudioGlobalTime = 0.0;
    double dAudioTimePerNESClock = 0.0; // Added this line
    double dAudioTimePerSystemSample = 0.0;

public: // Main Bus Read & Write
    void    cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

private:
    uint32_t nSystemClockCounter = 0;         // Counting the amount of clocks that have passed
    uint8_t controller_state[2];              // Controller State

private:
    uint8_t dma_page = 0x00;
    uint8_t dma_addr = 0x00;
    uint8_t dma_data = 0x00;
    bool dma_dummy = true;                    // Typo corrected
    bool dma_transfer = false;                // Flag indicating that DMA transfer is happening

public: // System Interface
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge); // Connects a cartridge object to the internal buses
    void reset();                                                      // Resets the system
    void clock();                                                      // Clocks the system - a single whole system tick
};
