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

    if (this->_jumps.empty()) {
        printf("ERROR - Jumps are empty after load\n");
        return false;
    }

    this->_currentJump = this->_jumps[0];
    this->_status = true;
    return true;
}

void Jumps::setStart(std::string scriptName)
{
    for (auto jump : this->_jumps) {
        if (jump->scriptName == scriptName) {
            this->_currentJump = jump;
            return;
        }
    }

    printf("Unable to set starting jump: %s\n", scriptName.c_str());
}

void Jumps::setCurrent(std::string scriptName)
{
    for (auto jump : this->_jumps) {
        if (jump->scriptName == scriptName) {
            this->_currentJump = jump;
            return;
        }
    }

    printf("Unable to set current jump: %s\n", scriptName.c_str());
}

void Jumps::move(int playerOption)
{
    /*
    this is how Jumps work:
    +------+----------+--------------+------------------+
    | id   | route_id | script       | jump_to_scene_id |
    +------+----------+-------+--------------+----------+
    |    1 |        0 | 00/00-00-A00 |                2 |
    |    2 |        0 | 00/00-00-A01 |                3 |
    |    3 |        0 | 00/00-00-A02 |                4 |
    |    4 |        0 | 00/00-00-A03 |                5 |
    |    4 |        0 | 00/00-00-A03 |                6 |
    |    4 |        0 | 00/00-00-A03 |                7 |
    |    5 |        0 | 00/00-00-A04 |                8 |
    |    6 |        0 | 00/00-00-A05 |                9 |
    */

    // find current jump and add move coursor if player set any option
    Jump* currJump = nullptr;
    for (int i = 0; i < this->_jumps.size(); i++) {
        if (this->_jumps[i] == this->_currentJump) {
            currJump = this->_jumps[i + playerOption]; // <- todo - check if jump is possible
        }
    }

    // check if current Jump was find
    if (currJump == nullptr) {
        printf("ERROR - Jump is impossible\n");
        return;
    }

    // move to next Jump
    for (auto jump : this->_jumps) {
        if (jump->id == currJump->jumpToId) {
            this->_currentJump = jump;
            return;
        }
    }

    // if next Jump is impossible to set
    printf("Unable to make jump\n");
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
