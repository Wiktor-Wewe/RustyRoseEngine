#include "Jumps.h"

Jumps::Jumps()
{
    this->_status = false;
	this->_currentJump = nullptr;
}

bool Jumps::load(std::string path)
{
    std::fstream file;
    file.open(path.c_str(), std::ios::in | std::ios::binary);

    if (!file.good()) {
        printf("ERROR - unable to read jumps: %s\n", path.c_str());
        return false;
    }

    uint32_t buffNum = 0x0000; char buffC[255];

    // check header
    memset(buffC, 0, 255);
    file.read(reinterpret_cast<char*>(&buffC), 6);

    if (strncmp(buffC, "BEsrcF", 6) != 0) {
        printf("ERROR - unable to read jumps | header error\n");
        file.close();
        return false;
    }

    while (!file.eof()) {
        Jump* jump = new Jump;

        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        jump->id = buffNum;

        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        jump->routeId = buffNum;

        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        memset(buffC, 0, 255);
        file.read(reinterpret_cast<char*>(buffC), buffNum);
        jump->scriptName = std::string(buffC);

        file.read(reinterpret_cast<char*>(&buffNum), sizeof(buffNum));
        jump->jumpToId = buffNum;

        this->_jumps.push_back(jump);

        if (file.eof()) {
            break;
        }
    }

    file.close();
    this->_status = true;
    return true;
}

void Jumps::move(int playerOption)
{
    for (int i = 0; i < this->_jumps.size(); i++) {
        if (this->_jumps[i] == this->_currentJump) {
            if (i + playerOption - 1 > this->_jumps.size()) {
                this->_currentJump = this->_jumps[this->_jumps.size() - 1]; // if jump is impossible, return last possible jump
                return;
            }
            this->_currentJump = this->_jumps[i + playerOption];
        }
    }
}

Jumps::Jump* Jumps::getCurrentJump()
{
    return this->_currentJump;
}

bool Jumps::getStatus()
{
    return this->_status;
}

Jumps::~Jumps()
{
    this->_currentJump = nullptr;
    for (auto jump : this->_jumps) {
        delete jump;
    }
}
