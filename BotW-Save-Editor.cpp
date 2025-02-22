//BotW Save Editor v1.1

#include <iostream>
#include <fstream>
#include <windows.h>
#include <rapidjson\document.h>
#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"
#include <rapidjson\prettywriter.h>
#include <cstdio>
#include <sstream>

void Import(std::string path, std::string name) {
	FILE* inJ = fopen(path.c_str(), "rb");

	fseek(inJ, 0, SEEK_END);
	size_t fileSize = ftell(inJ);
	fseek(inJ, 0, SEEK_SET);

	char* readBuffer = new char[fileSize];
	rapidjson::FileReadStream is(inJ, readBuffer, fileSize);

	rapidjson::Document doc;
	doc.ParseStream(is);

	fclose(inJ);

	int magic = _byteswap_ulong(doc["signature"].GetInt());
	int placeHolder = _byteswap_ulong(-1);
	int maybeVersion = _byteswap_ulong(1);

	const char* Path = path.erase(path.length() - 5, 5).c_str();
	std::fstream out;
	out.open(Path, std::fstream::out | std::fstream::ate | std::fstream::binary);
	out.seekg(0);
	out.write((const char*)&magic, 4);
	out.write((const char*)&placeHolder, 4);
	out.write((const char*)&maybeVersion, 4);

	for (unsigned int i = 0; i < doc["savdata"].Size(); i++)
	{
		int HashValue = _byteswap_ulong(doc["savdata"][i]["HashValue"].GetInt());
		out.write((const char*)&HashValue, 4);
		unsigned int DataValue = _byteswap_ulong(doc["savdata"][i]["DataValue"].GetUint());
		out.write((const char*)&DataValue, 4);
	}

	out.write((const char*)&placeHolder, 4);
	out.close();

	delete[] readBuffer;
}

void Export(std::string path, std::string name, int magic, const char* savdataExtension) {
	std::fstream in;
	in.open(path, std::fstream::in | std::fstream::ate | std::fstream::binary);
	int size = in.tellg();
	std::cout << "\nStarted exporting \"" << name << "\" to \"" << name + ".json" << "\" ..." << std::endl;
	in.seekg(12);

	wchar_t exePath[MAX_PATH]{};
	GetModuleFileName(NULL, exePath, MAX_PATH);
	std::wstring exePathWStr = exePath;
	std::string exePathStr = std::string(exePathWStr.begin(), exePathWStr.end());
	exePathStr = exePathStr.substr(0, exePathStr.find_last_of('\\'));
	std::string savdataList = exePathStr + std::string("\\savdata_list").append(savdataExtension).append(".json");

	FILE* inJ = fopen(savdataList.c_str(), "rb");

	fseek(inJ, 0, SEEK_END);
	size_t fileSize = ftell(inJ);
	fseek(inJ, 0, SEEK_SET);

	char* readBuffer = new char[fileSize];
	rapidjson::FileReadStream is(inJ, readBuffer, fileSize);

	rapidjson::Document doc;
	doc.ParseStream(is);

	fclose(inJ);

	std::string JSON = std::string("{\"signature\": ").append(std::to_string((int)_byteswap_ulong(magic))).append(",");

	JSON += "\"savdata\": []}";
	std::string savdata = "";

	uint32_t HashValue = 0;
	uint32_t DataValue = 0;
	for (int i = 0; i < (size - 16) / 8; i++)
	{
		in.read((char*)&HashValue, sizeof(HashValue));
		in.read((char*)&DataValue, sizeof(DataValue));
		if ((int)(_byteswap_ulong(HashValue)) < 0x7FFFFFFF)
		{
			savdata = "{\"DataName\": \"";
			savdata += doc[name.c_str()][std::to_string((int)(_byteswap_ulong(HashValue))).c_str()].GetString();
			savdata += "\",\"HashValue\": ";
			savdata += std::to_string((int)(_byteswap_ulong(HashValue)));
			savdata += ",\"DataValue\": ";
			savdata += std::to_string(_byteswap_ulong(DataValue));
			savdata += "}";
			JSON.insert(JSON.length() - 2, savdata);
			if (i < ((size - 16) / 8) - 1)
			{
				JSON.insert(JSON.length() - 2, ",");
			}
		}
	}

	in.close();

	const char* json = JSON.c_str();

	rapidjson::Document d;
	d.Parse(json);

	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	d.Accept(writer);

	FILE* out = fopen((name + ".json").c_str(), "wb");
	fwrite(sb.GetString(), 1, sb.GetSize(), out);
	fclose(out);

	delete[] readBuffer;
}

int main(int argc, char* argv[])
{
	std::cout << "\n\nBotW Save Editor by Mystixor. Big thanks to zephenryus, assisting to reverse engineer the save files.\n\n" << std::endl;
	switch (argc)
	{
	case 1:
	{
		std::cout << "Please use a *.sav file on the application." << std::endl;
		Sleep(1500);
		break;
	}
	case 2:
	{
		std::fstream in;
		in.open(argv[1], std::fstream::in | std::fstream::binary);
		uint32_t magic;
		in.read((char*)&magic, sizeof(magic));
		in.close();

		std::string filepath = argv[1];

		std::string filename = filepath;
		for (int f = filepath.length() - 1; f >= 0; f--)
		{
			if (filepath[f] == '/' || filepath[f] == '\\')
			{
				filename.erase(0, f + 1);
				std::cout << "Loaded file: \"" << filename << "\".\n" << std::endl;
				break;
			}
		}
		if (_byteswap_ulong(magic) == 18203)
		{
			std::cout << "Detected v1.5.0 save file!" << std::endl;
			Export(filepath, filename, magic, "471B");
		}
		else if (filename.find(".json") != std::string::npos)
		{
			std::cout << "Detected JSON!" << std::endl;
			Import(filepath, filename);
		}
		else if (_byteswap_ulong(magic) == 18202)
		{
			std::cout << "Detected v1.4.0+ save file!" << std::endl;
			Export(filepath, filename, magic, "471A");
		}
		else if (_byteswap_ulong(magic) == 10688)
		{
			std::cout << "Detected unsupported version! Please get in touch with me via Discord on the BotW Modding Hub." << std::endl;
			break;
		}
		else if (_byteswap_ulong(magic) == 9454)
		{
			std::cout << "Detected old save file! Please get in touch with me via Discord on the BotW Modding Hub." << std::endl;
			Export(filepath, filename, magic, "24EE");
		}
		else
		{
			std::cout << "Given file is not a save! Known files are:\n\ngame_data.sav\ncaption.sav\noption.sav" << std::endl;
			Sleep(3000);
			break;
		}
		break;
	}
	default:
	{
		std::cout << "Please supply only one file." << std::endl;
		Sleep(1500);
		break;
	}
	}
	return 0;
}
