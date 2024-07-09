#pragma once
#include <cstdint>
#include <array>

#include "olc6502.h"
#include "olc2C02.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on Main Bus

	// The 6502 derived processor
	olc6502 cpu;
	// The 2C02 Picture Processing Unit
	olc2C02 ppu;
	// The Cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;
	// 2KB of RAM
	uint8_t cpuRam[2048];

public: // Main Bus Read & Write
	void    cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

private:
	// A count of how many clocks have passed
	uint32_t nSystemClockCounter = 0;

public: // System Interface
	// Connects a cartridge object to the internal buses
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	// Resets the system
	void reset();
	// Clocks the system - a single whole systme tick
	void clock();
};
