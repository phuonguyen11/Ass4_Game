/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

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

#ifndef __PVP_H__
#define __PVP_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <network/SocketIO.h>

using namespace cocos2d::network;
using namespace cocos2d::ui;

USING_NS_CC;

class PvpMode : public cocos2d::Scene, public SocketIO::SIODelegate
{
private:
    bool turn = false;
    bool type = true;
    int size_of_caro = 64;
    Label* label_PVP = nullptr;
    Label* label_turn = nullptr;
    Menu* menu_PVP = nullptr;
    std::vector<std::vector<int>> board_number_PVP;
    SIOClient* _client = nullptr;
    std::vector<cocos2d::ui::Button*> buttonCollection;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void newGame(cocos2d::Ref* s);

    // socket.io event listener
    void JoinGameRoom(SIOClient* client);
    void ConnectToServerAndJoinRoom();
    void swapTurn(SIOClient* client, const std::string& data);
    void getType(SIOClient* client, const std::string& data);
    void onReceiveEvent(SIOClient* client, const std::string& data);
    void onFight(const cocos2d::Vec2& position);
    void returnWinner(int player);
    void showWinner(SIOClient* client, const std::string& player);
    void loadState(SIOClient* client, const std::string& pos);
    void clearBoard();
    void pauseGame(SIOClient* client, const std::string& data);
    void stopPause(SIOClient* client, const std::string& data);
    // SIODelegate
    virtual void onConnect(SIOClient* client);
    virtual void onMessage(SIOClient* client, const std::string& data);
    virtual void onClose(SIOClient* client);
    virtual void onError(SIOClient* client, const std::string& data);

    // implement the "static create()" method manually
    CREATE_FUNC(PvpMode);
};

#endif // __PVP_H__
