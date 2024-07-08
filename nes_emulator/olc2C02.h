#pragma once






class olc2C02
{
public:
	olc2C02();
	~olc2C02();

	//Communicate with Main Bus
	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void	cpuWrite(uint16_t addr, uint8_t data);

	//Communicate with PPU Bus
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void	ppuWrite(unit16_t addr, uint8_T data);
};

