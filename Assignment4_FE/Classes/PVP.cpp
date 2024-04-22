/*
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "PVP.h"
#include "Pause.h"
#include "Start.h"
#include "Node.h"
#include "End.h"

const int grid_pos_x = 0;
const int grid_pos_y = 0;

Scene* PvpMode::createScene()
{
    return PvpMode::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in PvpModeScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PvpMode::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    };

    this->ConnectToServerAndJoinRoom();

    // auto rule = Label::createWithTTF("Rule: player X go first", "fonts/richela.otf", 32);
    // rule->setPosition(Vec2(120, 730));
    // this->addChild(rule);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto director = Director::getInstance();

    this->label_turn = Label::createWithTTF("Wait for the opponent...", "fonts/richela.otf", 32);
    this->label_turn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        this->label_turn->getContentSize().height));
    this->addChild(this->label_turn);

    this->label_PVP = Label::createWithTTF("PVP", "fonts/richela.otf", 32);
    if (this->label_PVP == nullptr)
    {
        problemLoading("'fonts/richela.otf'");
    }
    else
    {
        // position the this->label_PVP on the center of the screen
        this->label_PVP->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - this->label_PVP->getContentSize().height));

        // add the this->label_PVP as a child to this layer
        this->addChild(this->label_PVP, 1);
    }

    for (int i = 0; i < 9; i++) {
        std::vector<int> v;
        for (int j = 0; j < 9; j++) {
            v.push_back(0);
        }
        this->board_number_PVP.push_back(v);
    }
    ////////////////////////-------------------------Check WINNER
    auto checkWinner = [this](int row, int col, bool t, int &player) -> bool {
        int temp;
        if (t) temp = 1;
        else temp = -1;
        this->board_number_PVP[row][col] = temp;
        std::string s = t ? "Chicken" : "Bunny";
        //int temp = this->board_number_PVP[row][col];

        //////////////-----------Vertical check
        int right_half = 0;
        int left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (row + i > 8 || this->board_number_PVP[row + i][col] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (row - i < 0 || this->board_number_PVP[row - i][col] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            if (s == "X") {
                player = 1;
            }
            else {
                player = 0;
            }
            return true;
        }
        //if (right_half + left_half >= 4) Director::getInstance()->stopAnimation();

        ///////////----------------Horizontal check
        right_half = 0;
        left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (col + i > 8 || this->board_number_PVP[row][col + i] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (col - i < 0 || this->board_number_PVP[row][col - i] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            if (s == "X") {
                player = 1;
            }
            else {
                player = 0;
            }
            return true;
        }
        //if (right_half + left_half >= 4) Director::getInstance()->pause();

        ///////////------------Main cross check
        right_half = 0;
        left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (col + i > 8 || row + i > 8 || this->board_number_PVP[row + i][col + i] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (col - i < 0 || row - i < 0 || this->board_number_PVP[row - i][col - i] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            if (s == "X") {
                player = 1;
            }
            else {
                player = 0;
            }
            return true;
        }

        ///////////------------Secondary cross check
        right_half = 0;
        left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (col + i > 8 || row - i < 0 || this->board_number_PVP[row - i][col + i] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (col - i < 0 || row + i > 8 || this->board_number_PVP[row + i][col - i] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            if (s == "X") {
                player = 1;
            }
            else {
                player = 0;
            }
            return true;
        }
        };

    auto randommove = [this]() -> std::vector<int> {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (this->board_number_PVP[i][j] == 0) {
                    std::vector<int> v;
                    v.push_back(i);
                    v.push_back(j);
                    return v;
                }
            }
        }
        };
    //////////////////////////// 9X9 BUTTON
    int start_pos_x = origin.x + visibleSize.width / 2 - this->size_of_caro * 4;
    int start_pos_y = origin.y + visibleSize.height / 2 + this->size_of_caro * 4;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            auto button = ui::Button::create();
            button->loadTextureNormal("normal.png");
            button->setOpacity(0);
            button->setTitleText("");
            button->setPosition(Vec2(start_pos_x + this->size_of_caro * j, start_pos_y));
            button->addClickEventListener([=](Ref* sender) {
                // Get the position of the button when clicked
                if (this->turn) {
                    int winner = 0;
                    if (this->type) button->loadTextureDisabled("chick_pvp.png");
                    else button->loadTextureDisabled("bunny_pvp.png");
                    button->getRendererDisabled()->setOpacity(255);
                    button->setEnabled(false);
                    button->setTitleColor(Color3B::BLACK);
                    if (checkWinner(i, j, (this->type ? 1 : 0), winner)) {
                        returnWinner(winner);
                    };

                    Vec2 position = button->getPosition();
                    std::string pos = button->getTitleText();

                    onFight(position);
                };
             });
            this->addChild(button, 1);
            this->buttonCollection.push_back(button);
        }
        start_pos_y -= this->size_of_caro;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(PvpMode::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    };

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto end = MenuItemImage::create("end.png", "end.png", CC_CALLBACK_1(PvpMode::newGame, this));
    if (end == nullptr ||
        end->getContentSize().width <= 0 ||
        end->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2;
        end->setPosition(Vec2(x, y));
    }
    this->menu_PVP = Menu::create(end, NULL);
    this->menu_PVP->setPosition(Vec2::ZERO);
    this->addChild(this->menu_PVP, 2);
    this->menu_PVP->setVisible(false);
    ////////////// BOARD
    auto grid = Sprite::create("grid_update.png");
    if (grid == nullptr)
    {
        problemLoading("'grid_update.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        grid->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(grid, 0);
    }

    return true;
}


void PvpMode::menuCloseCallback(Ref* pSender)
{
    std::vector<std::string> pauseData;
    pauseData.push_back("game-room");
    pauseData.push_back(std::to_string(this->type == 1));
    _client->emit("pause", pauseData);
}


void PvpMode::newGame(cocos2d::Ref* s) {
    auto newgame = End::createScene();
    Director::getInstance()->replaceScene(newgame);
    // return;
}
void PvpMode::onConnect(SIOClient* client) {
    // SocketIO::connect success
}
void PvpMode::onMessage(SIOClient* client, const std::string& data) {
    // SocketIO::send receive
}
void PvpMode::onClose(SIOClient* client) {
    // SocketIO::disconnect success
}
void PvpMode::onError(SIOClient* client, const std::string& data) {
    // SocketIO::failed
}

void PvpMode::onReceiveEvent(SIOClient* client, const std::string& data) {
    _client->emit("test", data);
};

void PvpMode::onFight(const cocos2d::Vec2& position) {
    std::vector<std::string> moveData;
    moveData.push_back("game-room");
    moveData.push_back(std::to_string(position.x));
    moveData.push_back(std::to_string(position.y));
    _client->emit("player_move", moveData);
    //_client->on("in_pause", CC_CALLBACK_1(PvpMode::stopPause, this));
};

void PvpMode::loadState(SIOClient* client, const std::string& pos) {
    int currposX = std::stoi(pos.substr(1, 3));
    int currposY = std::stoi(pos.substr(11, 3));
    for (const auto& button : this->buttonCollection) {
        if (button->getBoundingBox().containsPoint(Vec2(currposX, currposY))) {
            if (!this->type) {
                button->loadTextureNormal("chick_pvp.png");
                button->setOpacity(255);
            }
            else {
                button->loadTextureNormal("bunny_pvp.png");
                button->setOpacity(255);
            };
            button->setTouchEnabled(false);
            button->setTitleColor(Color3B::BLACK);
            break;
        }
    };
};

void PvpMode::swapTurn(SIOClient* client, const std::string& data) {
    if (std::stoi(data) == 1) {
        this->turn = true;
        this->label_turn->setString("Your turn");
    }
    else {
        this->turn = false;

        this->label_turn->setString("Wait for the next turn...");
    };
};

void PvpMode::getType(SIOClient* client, const std::string& data) {
    if (std::stoi(data) == 1) {
        this->type = true;
        auto lbl = Label::createWithTTF("You are \n Chicken", "fonts/richela.otf", 32);
        lbl->setPosition(Vec2(100, 400));
        this->addChild(lbl);

        this->label_turn->setString("Your turn");
    }
    else {
        this->type = false;
        auto lbl = Label::createWithTTF("You are \n Bunny", "fonts/richela.otf", 32);
        lbl->setPosition(Vec2(100, 400));
        this->addChild(lbl);

        this->label_turn->setString("Wait for the next turn...");
    };
    
};

void PvpMode::pauseGame(SIOClient* client, const std::string& data) {
    auto pause = Pause::createScene();
    Director::getInstance()->pushScene(pause);
};

void PvpMode::stopPause(SIOClient* client, const std::string& data) {
    Director::getInstance()->popScene();
};

void PvpMode::returnWinner(int player) {
    std::vector<std::string> moveData;
    moveData.push_back("game-room");
    moveData.push_back(std::to_string(player));
    _client->emit("winner", moveData);
};

void PvpMode::showWinner(SIOClient* client, const std::string& player) {
    if ((this->type && player[1] == '1') || (!this->type && player[1] == '0')) {
        this->label_PVP->setString("You has collected all the eggs!");
    }
    else {
        this->label_PVP->setString("You lose!");
    };

    this->menu_PVP->setVisible(true);
};

void PvpMode::JoinGameRoom(SIOClient* client)
{
    std::string roomName = "game-room";
    client->emit("join_room", roomName);
};

void PvpMode::ConnectToServerAndJoinRoom() {
    // connect to server
    _client = SocketIO::connect("http://localhost:8080", *this);
    _client->on("swap_turn", CC_CALLBACK_2(PvpMode::swapTurn, this));
    _client->on("get_type", CC_CALLBACK_2(PvpMode::getType, this));
    _client->on("connect", CC_CALLBACK_1(PvpMode::JoinGameRoom, this));
    _client->on("player_move", CC_CALLBACK_2(PvpMode::loadState, this));
    _client->on("winner", CC_CALLBACK_2(PvpMode::showWinner, this));
    _client->on("pause", CC_CALLBACK_2(PvpMode::pauseGame, this));
    _client->on("stop_pause", CC_CALLBACK_2(PvpMode::stopPause, this));
};

void PvpMode::clearBoard() {
    this->turn = false;
    this->type = true;
    this->size_of_caro = 64;
    this->label_PVP = nullptr;
    this->label_turn = nullptr;
    this->menu_PVP = nullptr;
    this->_client->disconnect();
    this->_client = nullptr;

    for (int i = 0; i < 9; i++) {
        this->board_number_PVP[i].clear();
    };
    this->board_number_PVP.clear();

    this->buttonCollection.clear();
};