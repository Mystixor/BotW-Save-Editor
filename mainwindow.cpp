#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <qmessagebox.h>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
#include <stdio.h>

using namespace std;
using namespace rapidjson;

int posMaxHart = 0, posCurrentHart = 0, posMasterMode = 0;
Document docGameData, docCaption;

void setupGameData() {
    ifstream file("game_data.sav.json");
    string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *cstr = str.c_str();

    if (docGameData.Parse(cstr).HasParseError()) {
        fprintf(stderr, "\nError(offset %u): %s\n",
            (unsigned)docGameData.GetErrorOffset(),
            GetParseError_En(docGameData.GetParseError()));
    }

    for(unsigned int i = 0; i < docGameData["savdata"].Size(); i++) {
        string tmpSavdata = docGameData["savdata"][i]["DataName"].GetString();

        if(tmpSavdata == "MaxHartValue") {
            posMaxHart = i;
        }else if(tmpSavdata == "CurrentHart") {
            posCurrentHart = i;
        }
    }
}

void setupCaption() {
    ifstream file("caption.sav.json");
    string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *cstr = str.c_str();

    if (docCaption.Parse(cstr).HasParseError()) {
        fprintf(stderr, "\nError(offset %u): %s\n",
            (unsigned)docCaption.GetErrorOffset(),
            GetParseError_En(docCaption.GetParseError()));
    }

    for(unsigned int i = 0; i < docCaption["savdata"].Size(); i++) {
        string tmpCaption = docCaption["savdata"][i]["DataName"].GetString();

        if(tmpCaption == "AoC_HardMode_Enabled") {
            posMasterMode = i;
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    ifstream optionF("option.sav.json");
    ifstream game_dataF("game_data.sav.json");
    ifstream captionF("caption.sav.json");
    if(!optionF || !game_dataF || !captionF) {
        QMessageBox::information(
            this,
            tr("Save files not found"),
            tr("Please unpack the save files (option.sav/game_data.sav/caption.sav). \nShow the README.md at https://github.com/Mystixor/BotW-Save-Editor"));
        exit(1);
    }

    setupGameData();
    setupCaption();

    ui->spinboxCurrentHeart->setValue(long(docGameData["savdata"][posCurrentHart]["DataValue"].GetInt()));
    ui->spinboxMaxHeart->setValue(long(docGameData["savdata"][posMaxHart]["DataValue"].GetInt()));

    ui->checkBoxMasterMode->setChecked(docCaption["savdata"][posMasterMode]["DataValue"].GetInt());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSave_clicked()
{
    docGameData["savdata"][posMaxHart]["DataValue"].SetInt(ui->spinboxMaxHeart->value());
    docGameData["savdata"][posCurrentHart]["DataValue"].SetInt(ui->spinboxCurrentHeart->value());

    remove("game_data.sav.json");
    remove("caption.sav.json");

    char writeBuffer[65536];

    string tmpGameData = string("game_data.sav.json");
    string tmpCaption = string("caption.sav.json");

    const char *GameData = tmpGameData.c_str();
    const char *Caption = tmpCaption.c_str();

    FILE* outGameData = fopen(GameData, "wb");
        FileWriteStream osGameData(outGameData, writeBuffer, sizeof(writeBuffer));
        PrettyWriter<FileWriteStream> writerGameData(osGameData);
        docGameData.Accept(writerGameData);
        fclose(outGameData);

    char writeBufferZ[65536];

    FILE* outCaption = fopen(Caption, "wb");
        FileWriteStream osCaption(outCaption, writeBufferZ, sizeof(writeBufferZ));
        PrettyWriter<FileWriteStream> writerCaption(osCaption);
        docCaption.Accept(writerCaption);
        fclose(outCaption);
}

void MainWindow::on_checkBoxMasterMode_stateChanged(int arg1)
{
    if(arg1 == 0) {
        docCaption["savdata"][posMasterMode]["DataValue"].SetInt(0);
    }else if(arg1 == 2) {
        docCaption["savdata"][posMasterMode]["DataValue"].SetInt(1);
    }
}
