#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main()
{
    std::string path = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data";
    std::string command = "dir " + path + " /B /S > " + path + "\\temp.rose";
    system(command.c_str());

    std::string filePath = path + "\\temp.rose";
    std::fstream fileList(filePath.c_str(), std::ios::in);
    
    if (!fileList.good()) {
        printf("cant open file temp.rose\n");
        return 1;
    }

    std::vector<std::string> bgm;
    std::vector<std::string> sysse;
    std::vector<std::string> system;
    std::vector<std::string> script;

    std::vector<std::string> event00;
    std::vector<std::string> event01;
    std::vector<std::string> event02;
    std::vector<std::string> event03;
    std::vector<std::string> event04;
    std::vector<std::string> event05;

    std::vector<std::string> movie00;
    std::vector<std::string> movie01;
    std::vector<std::string> movie02;
    std::vector<std::string> movie03;
    std::vector<std::string> movie04;
    std::vector<std::string> movie05;

    std::vector<std::string> se00;
    std::vector<std::string> se01;
    std::vector<std::string> se02;
    std::vector<std::string> se03;
    std::vector<std::string> se04;
    std::vector<std::string> se05;

    std::vector<std::string> voice00;
    std::vector<std::string> voice01;
    std::vector<std::string> voice02;
    std::vector<std::string> voice03;
    std::vector<std::string> voice04;
    std::vector<std::string> voice05;


    std::string line;
    while (!fileList.eof()) {
        std::getline(fileList, line);

        if (line.find("BGM.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            bgm.push_back(line);
        }
        else if (line.find("SysSe.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            sysse.push_back(line);
        }
        else if (line.find("System.GPK~") != std::string::npos && line.find(".PNG") != std::string::npos) {
            system.push_back(line);
        }
        else if (line.find("System.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            system.push_back(line);
        }
        else if (line.find("System.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            system.push_back(line);
        }
        else if (line.find("Script.GPK~") != std::string::npos && line.find(".ORS") != std::string::npos) {
            script.push_back(line);
        }
        else if (line.find("Event00.GPK~") != std::string::npos && line.find(".PNG") != std::string::npos) {
            event00.push_back(line);
        }
        else if (line.find("Event01.GPK~") != std::string::npos && line.find(".PNG") != std::string::npos) {
            event01.push_back(line);
        }
        else if (line.find("Event02.GPK~") != std::string::npos && line.find(".PNG") != std::string::npos) {
            event02.push_back(line);
        }
        else if (line.find("Event03.GPK~") != std::string::npos && line.find(".PNG") != std::string::npos) {
            event03.push_back(line);
        }
        else if (line.find("Event04.GPK~") != std::string::npos && line.find(".PNG") != std::string::npos) {
            event04.push_back(line);
        }
        else if (line.find("Event05.GPK~") != std::string::npos && line.find(".PNG") != std::string::npos) {
            event05.push_back(line);
        }
        else if (line.find("Movie00.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            movie00.push_back(line);
        }
        else if (line.find("Movie01.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            movie01.push_back(line);
        }
        else if (line.find("Movie02.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            movie02.push_back(line);
        }
        else if (line.find("Movie03.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            movie03.push_back(line);
        }
        else if (line.find("Movie04.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            movie04.push_back(line);
        }
        else if (line.find("Movie05.GPK~") != std::string::npos && line.find(".WMV") != std::string::npos) {
            movie05.push_back(line);
        }
        else if (line.find("Se00.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            se00.push_back(line);
        }
        else if (line.find("Se01.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            se01.push_back(line);
        }
        else if (line.find("Se02.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            se02.push_back(line);
        }
        else if (line.find("Se03.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            se03.push_back(line);
        }
        else if (line.find("Se04.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            se04.push_back(line);
        }
        else if (line.find("Se05.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            se05.push_back(line);
        }
        else if (line.find("Voice00.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            voice00.push_back(line);
        }
        else if (line.find("Voice01.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            voice01.push_back(line);
        }
        else if (line.find("Voice02.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            voice02.push_back(line);
        }
        else if (line.find("Voice03.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            voice03.push_back(line);
        }
        else if (line.find("Voice04.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            voice04.push_back(line);
        }
        else if (line.find("Voice05.GPK~") != std::string::npos && line.find(".OGG") != std::string::npos) {
            voice05.push_back(line);
        }
    }

    int dummy;

    fileList.close();
    return 0;
}
