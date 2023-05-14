// WinConsole_Playground.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "iBlowFish.h"

bool ReadFile(std::string filename, std::vector<unsigned char>& bytes);
bool WriteFile(std::string filename, std::vector<unsigned char>& bytes);
void XOREncrypt(std::vector<unsigned char>& bytes, std::string key);

int main(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}

	if (argc < 5)
	{
		std::cout << "Not enough arguments" << std::endl;
		return 0;
	}

	std::string key = argv[1];
	std::string filename_source = argv[2];
	std::string filename_encrypt = argv[3];
	std::string filename_decrypt = argv[4];

	// init blowfish
	CBLOWFISH blowfish;
	blowfish.InitKey((unsigned char*)key.c_str(), key.size());
	
	// encrypt
	{
		std::vector<unsigned char> bytes;
		if (ReadFile(filename_source, bytes))
		{
			//XOREncrypt(bytes, key);
			bool success = blowfish.EncryptData(bytes.data(), bytes.size(), bytes.data(), bytes.size());
			
			if (success)
			{
				WriteFile(filename_encrypt, bytes);
			}
		}
	}

	// decrypt
	{
		std::vector<unsigned char> bytes;
		if (ReadFile(filename_encrypt, bytes))
		{
			bool success = false;
			blowfish.DecryptData(bytes.data(), bytes.size(), bytes.data(), bytes.size());
			//XOREncrypt(bytes, key);
			WriteFile(filename_decrypt, bytes);
		}
	}

	return 0;
}

bool ReadFile(std::string filename, std::vector<unsigned char>& bytes)
{
	std::ifstream ifile;
	ifile.open(filename, std::ios::binary);
	if (ifile.is_open())
	{
		ifile.seekg(0, std::ios::end);
		std::streampos size = ifile.tellg();
		ifile.seekg(0, std::ios::beg);

		bytes.resize(size);
		ifile.read((char*)bytes.data(), size);
		ifile.close();

		//// print 
		//for (int i = 0; i < bytes.size(); i++)
		//{
		//	std::cout << bytes[i];
		//}

		return true;
	}

	std::cout << "Unable to open ifile";
	return false;
}

bool WriteFile(std::string filename, std::vector<unsigned char>& bytes)
{
	//// print bytes
	//for (int i = 0; i < bytes.size(); i++)
	//{
	//	std::cout << bytes[i];
	//}

	std::ofstream ofile;
	ofile.open(filename, std::ios::binary);
	if (ofile.is_open())
	{
		ofile.write((char*)bytes.data(), bytes.size());
		ofile.close();
		return true;
	}

	std::cout << "Unable to open ofile";
	return false;
}

// XOR encrypt function
void XOREncrypt(std::vector<unsigned char>& bytes, std::string key)
{
	for (int i = 0; i < bytes.size(); i++)
	{
		bytes[i] = bytes[i] ^ key[i % key.size()];
	}
}
