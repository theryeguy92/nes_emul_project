#include "Bus.h"



Bus::Bus()
{
	//Clear RAM, just in case if we mess up
	for (auto &i : ram) i = 0x00;

	// Connect CPU to communication bus
	cpu.ConnectBus(this);

}

Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <=0xFFFF) //Guard RAM at a range, now it is the full range.
		ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		return ram[addr];

	return 0x00;
}