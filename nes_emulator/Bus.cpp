#include "Bus.h"



Bus::Bus()
{
	//Clear RAM, just in case if we mess up
	//for (auto &i : ram) i = 0x00;

	// Connect CPU to communication bus
	cpu.ConnectBus(this);

}

Bus::~Bus()
{
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
	if (cart->cpuWrite(addr, data))
	{
		//Put comments here to describe what is happening
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		//Comments to describe what is happening
		cpuRam[addr & 0x07FF] = data;
		
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		//Comments here
		ppu.cpuWrite(addr & 0x0007, data);
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	uint8_t data = 0x00;
	if (cart->cpuRead(addr, data))
	{
		//Cartridge Address Range
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		//System RAM address Range, mirrored every 2048 *Talk about mirroring*
		data = cpuRam[addr & 0x07FF];
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		// PPU Address range, mirrored every 8
		data = ppu.cpuRead(addr & 0x0007, bReadOnly);
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	// This Connects the cartridge to both the Main Bus and the CPU Bus
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	ppu.clock();



	if (nSystemClockCounter % 3 == 0) 
	{
		cpu.clock();
	}

	nSystemClockCounter++;
}