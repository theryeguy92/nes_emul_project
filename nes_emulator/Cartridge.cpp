#include "Cartridge.h"

Cartridge::Cartridge(const std::string& sFileName)
{
	//iNES Format Header
	struct sHeader
	{
		char name[4];
		uint8_t prg_rom_chunks;
		uint8_t chr_rom_chunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char unused[5];

	} header;

	bImageValid = false;

	std::ifstream ifs;
	ifs.open(sFileName, std::ifstream::binary);
	if (ifs.isopen())
	{
		//Read file header
		ifs.read((char*)&header, sizeof(sHeader));

		//read past trainer
		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::curl);

		//Determine Mapper ID
		nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// "Discover" the File Format
		uint8_t nFileType = 1;

		if (nFileType == 0)
		{

		}
		if (nFileType == 2)
		{

		}
		//Load appropriate mapper
		switch (nMapperID)
		{
		case 0: pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks); break;
		}

		bImageValid = true;
		ifs.close();
	}

}

Cartridge::~Cartridge()
{
}

bool Cartridge::ImageValid()
{
	return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapRead(addr, mapped_addr))
	{
		data = vPRGMemory[mapped_addr];
		return true;
	}
	else
		return false;

}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		vCHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}
