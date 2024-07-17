#include "Bus.h"

Bus::Bus()
{
	// Connect CPU to communication bus
	cpu.ConnectBus(this);

}


Bus::~Bus()
{
}

void Bus::SetSampleFrequency(uint32_t sample_rate)
{
	dAudioTimePerSystemSample = 1.0 / (double)sample_rate;
	dAudioTimePerNESClock = 1.0 / 5369318.0; //PPU Clock Frequency
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
	if (cart->cpuWrite(addr, data))
	{

		// Allows the cartridge to map any address to some
		// other data. This can help with Bank Switching.
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// System RAM Address Range: 8KB. The 2KB is "mirrored"
		// through this address range. Using bitwise AND to mask
		// the bottom 11 bits is the same as addr % 2048.
		cpuRam[addr & 0x07FF] = data;

	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		// PPU Address range. The PPU only has 8 primary registers
		// of which are repeated in this range.
		ppu.cpuWrite(addr & 0x0007, data);
	}
	else if ((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017) 
	{
		apu.cpuWrite(addr, data);
	}
	else if (addr == 0x4014)
	{
		//Writing to this address to initian a DMA transfer
		dma_page = data;
		dma_addr = 0x00;
		dma_transfer = true;
	}
	else if (adr >= 0x4016 && addr <= 0x4017)
	{
		//Here we are going to lock inthe controller state that this time
		controller_state[addr & 0x0001] = controller[addr & 0x0001];
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	uint8_t data = 0x00;
	if (cart->cpuRead(addr, data))
	{
		// Cartridge Address Range
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// System RAM Address Range, mirrored every 2048
		data = cpuRam[addr & 0x07FF];
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		// PPU Address range, mirrored every 8
		data = ppu.cpuRead(addr & 0x0007, bReadOnly);
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		data = (controller_state[addr & 0x0001] & 0x80) > 0;
		controller_state[addr & 0x0001] <<= 1;
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	// Connects cartridge to both Main Bus and CPU Bus
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset()
{
	cpu->reset();
	cpu.reset();
	ppu.reset();
	nSystemClockCounter = 0;
	dma_page = 0x00;
	dma_addr = 0x00;
	dma_data = 0x00;
	dma_dummy = true;
	dma_transfer = false;
}

bool Bus::clock()
{

	// The fastest clock frequency the digital system cares
	// about is equivalent to the PPU clock. So the PPU is clocked
	// each time this function is called...
	ppu.clock();


	apu.clock();

	// The CPU runs 3 times slower than the PPU so we only call its
	// clock() function every 3 times this function is called. .
	if (nSystemClockCounter % 3 == 0)
	{
		// Is the system performing a DMA transfer form CPU memory to 
		// OAM memory on PPU?...
		if (dma_transfer)
		{
			// ...Yes! We need to wait until the next even CPU clock cycle
			// before it starts...
			if (dma_dummy)
			{
				// ...So hang around in here each clock until 1 or 2 cycles
				// have elapsed...
				if (nSystemClockCounter % 2 == 1)
				{
					// ...and finally allow DMA to start
					dma_dummy = false;
				}
			}
			else
			{
				// DMA can take place!
				if (nSystemClockCounter % 2 == 0)
				{
					// On even clock cycles, read from CPU bus
					dma_data = cpuRead(dma_page << 8 | dma_addr);
				}
				else
				{

					ppu.pOAM[dma_addr] = dma_data;

					dma_addr++;


					if (dma_addr == 0x00)
					{
						dma_transfer = false;
						dma_dummy = true;
					}
				}
			}
		}
		else
		{
			// No DMA happening
			cpu.clock();
		}
	}

	// Synchronising with Audio
	bool bAudioSampleReady = false;
	dAudioTime += dAudioTimePerNESClock;
	if (dAudioTime >= dAudioTimePerSystemSample)
	{
		dAudioTime -= dAudioTimePerSystemSample;
		dAudioSample = apu.GetOutputSample();
		bAudioSampleReady = true;
	}

	// The PPU is capable of emitting an interrupt to indicate the
	// vertical blanking period has been entered. If it has, we need
	// to send that irq to the CPU.
	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nmi();
	}


	// Check if cartridge is requesting IRQ
	if (cart->GetMapper()->irqState())
	{
		cart->GetMapper()->irqClear();
		cpu.irq();
	}

	nSystemClockCounter++;

	return bAudioSampleReady;
}