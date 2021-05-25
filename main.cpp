//Linux compiler command: g++ -std=c++17 main.cpp -fpermissive -o editor

#ifdef __unix__
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(WIN32)
#pragma comment(lib, "Ws2_32.lib")
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <winsock.h>
#endif
#include <iomanip>
#include <fstream>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

void csleep(int microseconds) {
#ifdef __unix__
    usleep(microseconds);
#elif defined(_WIN32) || defined(WIN32)
    Sleep(microseconds);
#endif
}

void Import(string path, string name) {

}

void Export(string path, string name, int magic, const char* savedataExtension) {
    fstream in;
    in.open(path, fstream::in | fstream::ate | fstream::binary);
    int size = in.tellg();
    cout << endl << "Started exporting \"" << name << "\" to \"" << name + ".json" << "\" ..." << endl;
    in.seekg(12);

    string savedataList = string("savdata_list").append(savedataExtension).append(".json");

    ifstream savedataListIF(savedataList);
    if(savedataListIF.fail()) {
        cout << savedataList << " not found or access denied!" << endl;
        return;
    }
    
    json savedataListJson;
    savedataListIF >> savedataListJson;
    savedataListIF.close();

    json saveOutputJson;
    saveOutputJson["signature"] = to_string((int)htonl(magic));
    
    char *HashValue;
    char *DataValue;
    json savedataArray = json::array();

    for(int i = 0; i < (size - 16) / 8; i++) {
        in.read((char*) &HashValue, sizeof(HashValue));
        in.read((char*) &DataValue, sizeof(DataValue));
        int hash = (int)(htonl((unsigned int) HashValue));
        if(hash < 2147483647) {
            json savedataObject = json::object();

            savedataObject["DataName"] = savedataListJson[name][to_string(hash)];
            savedataObject["HashValue"] = to_string(hash);
            savedataObject["DataValue"] = to_string(htonl((unsigned int) DataValue));

            savedataArray.push_back(savedataObject);
        }
    }
    saveOutputJson["savdata"] = savedataArray;
    
    in.close();

    ofstream output((name + ".json").c_str());
    output << std::setw(4) << saveOutputJson << endl;
    output.flush();
    output.close();
}

int main(int argc, char* argv[]) {
    cout << endl << endl << "BotW Save Editor by Mystixor. Big thanks to zephenryus, assisting to reverse engineer the save files." << endl << endl << endl;
    switch (argc) {
    case 1:
        cout << "Please use a *.sav file on the application." << endl;
        csleep(1500);
        break;

    case 2:
        fstream in;
        in.open(argv[1], fstream::in | fstream::binary);
        uint32_t magic;
        in.read((char*)&magic, sizeof(magic));
        in.close();

        string filepath = argv[1];
        string filename = filepath;

        for (int i = filepath.length() - 1; i >= 0; i--) {
            if (filepath[i] == '/' || filepath[i] == '\\') {
                filename.erase(0, i + 1);
                cout << "Loaded file: \"" << filename << "\"." << endl << endl;
                break;
            }
        }
        if (htonl(magic) == 18203) {
            cout << "Detected v1.5.0 save file!" << endl;
            Export(filepath, filename, magic, "471B");
        }
        else if (htonl(magic) == 18202) {
            cout << "Detected 1.4.0+ save file!" << endl;
            Export(filepath, filename, magic, "471A");
        }
        else if (htonl(magic) == 10688) {
            cout << "Detected unsupported version! Please get in touch with me via Discord on the BotW Modding Hub." << endl;
            break;
        }
        else if (htonl(magic) == 9454) {
            cout << "Detected old save file! Please get in touch with me via Discord on the BotW Modding Hub." << endl;
            Export(filepath, filename, magic, "24EE");
        }
        else if (filename.find(".json") != string::npos) {
            cout << "Detected JSON!" << endl;
            Import(filepath, filename);
        }
        else {
            cout << "Given file is not a save! Known files are:" << endl << endl << "game_data.sav" << endl << "caption.sav" << endl << "option.sav" << endl;
            csleep(3000);
            break;
        }
        break;
    }
    return 0;
}
