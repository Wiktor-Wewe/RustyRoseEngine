#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main()
{
    std::string path = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data";
    std::string command = "dir " + path + " /B /S > " + path + "\\temp.rose";
    std::system(command.c_str());

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

    fileList.close();

    std::string fileOutPath = path + "\\list.rose";
    std::fstream fileOut(fileOutPath.c_str(), std::ios::out);

    fileOut << "BGM" << std::endl;
    fileOut << bgm.size() << std::endl;
    for (int i = 0; i < bgm.size(); i++) {
        fileOut << bgm[i] << std::endl;
    }

    fileOut << "SysSe" << std::endl;
    fileOut << sysse.size() << std::endl;
    for (int i = 0; i < sysse.size(); i++) {
        fileOut << sysse[i] << std::endl;
    }

    fileOut << "System" << std::endl;
    fileOut << system.size() << std::endl;
    for (int i = 0; i < system.size(); i++) {
        fileOut << system[i] << std::endl;
    }

    fileOut << "Script" << std::endl;
    fileOut << script.size() << std::endl;
    for (int i = 0; i < script.size(); i++) {
        fileOut << script[i] << std::endl;
    }
    
    fileOut << "Event00" << std::endl;
    fileOut << event00.size() << std::endl;
    for (int i = 0; i < event00.size(); i++) {
        fileOut << event00[i] << std::endl;
    }
    
    fileOut << "Event01" << std::endl;
    fileOut << event01.size() << std::endl;
    for (int i = 0; i < event01.size(); i++) {
        fileOut << event01[i] << std::endl;
    }

    fileOut << "Event02" << std::endl;
    fileOut << event02.size() << std::endl;
    for (int i = 0; i < event02.size(); i++) {
        fileOut << event02[i] << std::endl;
    }

    fileOut << "Event03" << std::endl;
    fileOut << event03.size() << std::endl;
    for (int i = 0; i < event03.size(); i++) {
        fileOut << event03[i] << std::endl;
    }

    fileOut << "Event04" << std::endl;
    fileOut << event04.size() << std::endl;
    for (int i = 0; i < event04.size(); i++) {
        fileOut << event04[i] << std::endl;
    }

    fileOut << "Event05" << std::endl;
    fileOut << event05.size() << std::endl;
    for (int i = 0; i < event05.size(); i++) {
        fileOut << event05[i] << std::endl;
    }

    fileOut << "Movie00" << std::endl;
    fileOut << movie00.size() << std::endl;
    for (int i = 0; i < movie00.size(); i++) {
        fileOut << movie00[i] << std::endl;
    }

    fileOut << "Movie01" << std::endl;
    fileOut << movie01.size() << std::endl;
    for (int i = 0; i < movie01.size(); i++) {
        fileOut << movie01[i] << std::endl;
    }
    
    fileOut << "Movie02" << std::endl;
    fileOut << movie02.size() << std::endl;
    for (int i = 0; i < movie02.size(); i++) {
        fileOut << movie02[i] << std::endl;
    }

    fileOut << "Movie03" << std::endl;
    fileOut << movie03.size() << std::endl;
    for (int i = 0; i < movie03.size(); i++) {
        fileOut << movie03[i] << std::endl;
    }

    fileOut << "Movie04" << std::endl;
    fileOut << movie04.size() << std::endl;
    for (int i = 0; i < movie04.size(); i++) {
        fileOut << movie04[i] << std::endl;
    }

    fileOut << "Movie05" << std::endl;
    fileOut << movie05.size() << std::endl;
    for (int i = 0; i < movie05.size(); i++) {
        fileOut << movie05[i] << std::endl;
    }
    
    fileOut << "Se00" << std::endl;
    fileOut << se00.size() << std::endl;
    for (int i = 0; i < se00.size(); i++) {
        fileOut << se00[i] << std::endl;
    }
    
    fileOut << "Se01" << std::endl;
    fileOut << se01.size() << std::endl;
    for (int i = 0; i < se01.size(); i++) {
        fileOut << se01[i] << std::endl;
    }

    fileOut << "Se02" << std::endl;
    fileOut << se02.size() << std::endl;
    for (int i = 0; i < se02.size(); i++) {
        fileOut << se02[i] << std::endl;
    }

    fileOut << "Se03" << std::endl;
    fileOut << se03.size() << std::endl;
    for (int i = 0; i < se03.size(); i++) {
        fileOut << se03[i] << std::endl;
    }

    fileOut << "Se04" << std::endl;
    fileOut << se04.size() << std::endl;
    for (int i = 0; i < se04.size(); i++) {
        fileOut << se04[i] << std::endl;
    }

    fileOut << "Se05" << std::endl;
    fileOut << se05.size() << std::endl;
    for (int i = 0; i < se05.size(); i++) {
        fileOut << se05[i] << std::endl;
    }

    fileOut << "Voice00" << std::endl;
    fileOut << voice00.size() << std::endl;
    for (int i = 0; i < voice00.size(); i++) {
        fileOut << voice00[i] << std::endl;
    }

    fileOut << "Voice01" << std::endl;
    fileOut << voice01.size() << std::endl;
    for (int i = 0; i < voice01.size(); i++) {
        fileOut << voice01[i] << std::endl;
    }

    fileOut << "Voice02" << std::endl;
    fileOut << voice02.size() << std::endl;
    for (int i = 0; i < voice02.size(); i++) {
        fileOut << voice02[i] << std::endl;
    }

    fileOut << "Voice03" << std::endl;
    fileOut << voice03.size() << std::endl;
    for (int i = 0; i < voice03.size(); i++) {
        fileOut << voice03[i] << std::endl;
    }

    fileOut << "Voice04" << std::endl;
    fileOut << voice04.size() << std::endl;
    for (int i = 0; i < voice04.size(); i++) {
        fileOut << voice04[i] << std::endl;
    }

    fileOut << "Voice05" << std::endl;
    fileOut << voice05.size() << std::endl;
    for (int i = 0; i < voice05.size(); i++) {
        fileOut << voice05[i] << std::endl;
    }

    std::system("del data\\temp.rose");
    fileOut.close();
    return 0;
}
