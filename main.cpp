#include <iostream>
#include <fstream>
#include <windows.h>
#include <rapidjson\document.h>
#include "rapidjson\filereadstream.h"
#include "rapidjson\filewritestream.h"
#include <rapidjson\prettywriter.h>
#include <cstdio>
#include <sstream>

using namespace std;
using namespace rapidjson;

void Import(string path, string name) {
	FILE* inJ = fopen(path.c_str(), "rb");
	char readBuffer[65536];
	FileReadStream is(inJ, readBuffer, sizeof(readBuffer));
	Document doc;
	doc.ParseStream(is);
	fclose(inJ);

	int magic = htonl(18203);
	int placeHolder = htonl(-1);
	int maybeVersion = htonl(1);

	const char* Path = path.erase(path.length()-5, 5).c_str();
	fstream out;
	out.open(Path, std::fstream::out | std::fstream::ate | std::fstream::binary);
	out.seekg(0);
	out.write((const char*)&magic, 4);
	out.write((const char*)&placeHolder, 4);
	out.write((const char*)&maybeVersion, 4);

	for(unsigned int i = 0; i < doc["savdata"].Size(); i++)
	{
		int HashValue = htonl(doc["savdata"][i]["HashValue"].GetInt());
		out.write((const char*)&HashValue, 4);
		unsigned int DataValue = htonl(doc["savdata"][i]["DataValue"].GetUint());
		out.write((const char*)&DataValue, 4);
	}

	out.write((const char*)&placeHolder, 4);
	out.close();
}

void Export(string path, string name) {
	fstream in;
	in.open(path, std::fstream::in | std::fstream::ate | std::fstream::binary);
	int size = in.tellg();
	cout << "\nStarted exporting \"" << name << "\" to \"" << name + ".json" <<"\" ..." << endl;
	in.seekg(12);

	FILE* inJ = fopen("savdata_list.json", "rb");
	char readBuffer[65536];
	FileReadStream is(inJ, readBuffer, sizeof(readBuffer));
	Document doc;
	doc.ParseStream(is);
	fclose(inJ);

	string JSON = "{\"savdata\": []}";
	char *HashValue;
	char *DataValue;
	string savdata = "";

	for(int i = 0; i < (size - 16) / 8; i++)
	{
		in.read((char*)&HashValue, sizeof(HashValue));
		in.read((char*)&DataValue, sizeof(DataValue));
		if((int)(htonl((unsigned int)HashValue)) < 2147483647)
		{
			savdata = "{\"DataName\": \"";
			savdata += doc[name.c_str()][to_string((int)(htonl((unsigned int)HashValue))).c_str()].GetString();
			savdata += "\",\"HashValue\": ";
			savdata += to_string((int)(htonl((unsigned int)HashValue)));
			savdata += ",\"DataValue\": ";
			savdata += to_string(htonl((unsigned int)DataValue));
			savdata += "}";
			JSON.insert(JSON.length()-2, savdata);
			if(i < ((size - 16) / 8) - 1)
			{
				JSON.insert(JSON.length()-2, ",");
			}
		}
	}

	in.close();

	const char *json = JSON.c_str();

	Document d;
	d.Parse(json);

	FILE* out = fopen((name + ".json").c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(out, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(out);
}

int main(int argc, char *argv[])
{
	cout << "\n\nBotW Save Editor by Mystixor. Big thanks to zephenryus, assisting to reverse engineer the save files.\n\n" << endl;
	switch(argc)
	{
		case 1:
		{
			cout << "Please use a *.sav file on the application." << endl;
			Sleep(1500);
			break;
		}
		case 2:
		{
			fstream in;
			in.open(argv[1], std::fstream::in | std::fstream::binary);
			uint32_t magic;
			in.read((char*)&magic, sizeof(magic));
			in.close();

			string filepath = argv[1];

			string filename = filepath;
			for(int f = filepath.length() - 1; f >= 0; f--)
			{
				if(filepath[f] == '/' || filepath[f] == '\\')
				{
					filename.erase(0, f+1);
					cout << "Loaded file: \"" << filename << "\".\n" << endl;
					break;
				}
			}
			if(htonl(magic) == 18203)
			{
				cout << "Detected save file!" << endl;
				Export(filepath, filename);
			}
			else if(filename.find(".json") != string::npos)
			{
				cout << "Detected JSON!" << endl;
				Import(filepath, filename);
			}
			else
			{
				cout << "Given file is not a save! Known files are:\n\ngame_data.sav\ncaption.sav\noption.sav" << endl;
				Sleep(3000);
				break;
			}
			break;
		}
		default:
		{
			cout << "Please supply only one file." << endl;
			Sleep(1500);
			break;
		}
	}
	return 0;
}
