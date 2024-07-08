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

public: // Devices on Main bus
	//The Processor
	olc6502 cpu;

	// The 2C02 PPU (Picture Processing Unit)
	olc2C02 ppu;

	// The Cartridge or "GamePak"
	std::shared_ptr<Cartridge> cart;

	// 2KB of RAM
	uint8_t cpuRam[2048];

	// Fake RAM for this part of the series
	//std::array<uint8_t, 2048> cpuram; //change to 2kb


public: // Main Bus Read & Write
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

private:
	// Count how many clocks have passed
	uint32_t nSystemClockCounter = 0;

public: //System Interface
	//Connects a cartridge object to the internal bus
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	//Resets System
	void reset();
	//clocks the system - a single whole sys tick
	void clock();

};
