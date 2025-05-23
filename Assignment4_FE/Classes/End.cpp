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

#include "End.h"
#include "Start.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"

#include <iostream>
#include <sstream>
#include <string>

#include "network/WebSocket.h"
#include "network/SocketIO.h"
#include "cocos2d.h"
using namespace cocos2d;

USING_NS_CC;

int winner;

Scene* End::createScene()
{
    return End::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool End::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto director = Director::getInstance();

    auto sprite = Sprite::create("end_upgrade.png");

    if (sprite == nullptr)
    {
        problemLoading("'end_upgrade.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    auto label_13 = Label::createWithTTF("HAPPY EASTER DAY!", "fonts/richela.otf", 64);
    if (label_13 == nullptr)
    {
        problemLoading("'fonts/richela.otf'");
    }
    else
    {
        label_13->setTextColor(Color4B(6, 83, 116, 255));
        // position the label_1 on the center of the screen
        label_13->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label_13->getContentSize().height));

        // add the label_1 as a child to this layer
        this->addChild(label_13, 1);
    }




    Label* label_2;
    label_2 = Label::createWithTTF("Back To Menu", "fonts/richela.otf", 32);
    if (label_2 == nullptr)
    {
        problemLoading("'fonts/richela.otf'");
    }
    else
    {
        // position the label_2 on the center of the screen
        label_2->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height/2 - label_2->getContentSize().height + 210));

        // add the label_2 as a child to this layer
        this->addChild(label_2, 2);
    }

    Label* label_3;
    label_3 = Label::createWithTTF("Quit", "fonts/richela.otf", 32);
    if (label_3 == nullptr)
    {
        problemLoading("'fonts/richela.otf'");
    }
    else
    {
        // position the label_2 on the center of the screen
        label_3->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height/2 - label_2->getContentSize().height + 85));

        // add the label_2 as a child to this layer
        this->addChild(label_3, 2);
    }

    auto newItem = MenuItemImage::create(
                                           "item.png",
                                           "item.png",
                                           CC_CALLBACK_1(End::menuNewGame, this));

    if (newItem == nullptr ||
        newItem->getContentSize().width <= 0 ||
        newItem->getContentSize().height <= 0)
    {
        problemLoading("'item.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2 + 175;
        newItem->setPosition(Vec2(x,y));
    }


    auto closeItem = MenuItemImage::create(
                                           "item.png",
                                           "item.png",
                                           CC_CALLBACK_1(End::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'item.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height / 2 + 50;
        closeItem->setPosition(Vec2(x,y));
    }


    // create menu, it's an autorelease object
    auto menu = Menu::create(newItem, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /*
    /////////////////////////////
    // 3. add your codes below...

    // add a label_2 shows "Hello World"
    // create and initialize a label_2


    // add "HelloWorld" splash screen"
    return_1 true;
    */
    ////////////// BOARD
    
    return true;
}


void End::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    //Director::getInstance()->end();
    Director::getInstance()->popScene();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void End::menuNewGame(cocos2d::Ref* s) {
    auto newgame = Start::createScene();
    Director::getInstance()->replaceScene(newgame);
    // return;
}




