#pragma once
#include <cstdint>
#include "olc6502.h"


class Bus
{
public:
	Bus();
	~Bus();


public: //Devices on the bus
	olc6502 cpu;

	//For now, make fake RAM
	std::array<uint8_t, 64 * 1024> ram;

public: //Creating Class so the Bus can Read and Write Data
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool bReadOnly = false);

};

