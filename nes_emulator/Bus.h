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
	//Controller
	uint8_t controller[2];

	//include Sync with sytem Audio
public:
	void SetSampleFrequency(uint32_t sample_rate);
	double dAudioSample = 0.0;

private:
	double dAudioTime = 0.0;
	double dAudioGlobalTime = 0.0;
	double AudioTimePerNESClock = 0.0;
	double dAudioTimePerSystemSample = 0.0f;



public: // Main Bus Read & Write
	void    cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

private:
	// Counting the amount of clocks that have passed
	uint32_t nSystemClockCounter = 0;
	// Controller State
	uint8_t controller_state[2];



private:
	//Implimenting Direct Memory Access. Transfering CPU bus memory into OAM memory.
	//Some improvment could be made here (possibly)
	uint8_t dma_page = 0x00;
	uint8_t dma_addr = 0x00;
	uint8_t dma_data = 0x00;

	//CPU needs to be on an even clock cycle. For now, we will use a dummy cycle
	bool duma_dummy = true;

	//Flag indicating that DMA transfer is happening
	bool dma_transfer = false;

public: // System Interface
	// Connects a cartridge object to the internal buses
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	// Resets the system
	void reset();
	// Clocks the system - a single whole systme tick
	void clock();
};
