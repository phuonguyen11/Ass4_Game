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

#include "HelloWorldScene.h"
#include "Pause.h"
#include "Start.h"
#include "Node.h"
#include "End.h"


USING_NS_CC;

const int grid_pos_x = 0;
const int grid_pos_y = 0;
const int size_of_caro = 64;


bool turn_1;
std::vector<std::vector<int>> board_number_1;
std::vector<std::vector<Vec2>> caro_position_1;
std::vector<ui::Button*> button_grid_1;
Label* label_1;
Menu* menu_1;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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

    turn_1 = false;
    label_1 = Label::createWithTTF("Normal", "fonts/richela.otf", 32);
    if (label_1 == nullptr)
    {
        problemLoading("'fonts/richela.otf'");
    }
    else
    {
        // position the label_1 on the center of the screen
        label_1->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label_1->getContentSize().height));

        // add the label_1 as a child to this layer
        this->addChild(label_1, 1);
    }

    int start_pos_x = origin.x + visibleSize.width / 2 - size_of_caro * 4;
    int start_pos_y = origin.y + visibleSize.height / 2 + size_of_caro * 4;

    for (int i = 0; i < 9; i++) {
        std::vector<int> v;
        std::vector<Vec2> vec;
        for (int j = 0; j < 9; j++) {
            v.push_back(0);
            vec.push_back(Vec2(start_pos_x + size_of_caro * j, start_pos_y));
        }
        board_number_1.push_back(v);
        caro_position_1.push_back(vec);
        start_pos_y -= size_of_caro;
    }
    ////////////////////////-------------------------Check WINNER
    auto checkWinner = [this](int row, int col, bool t) -> bool {
        int temp;
        if (t) temp = 1;
        else temp = -1;
        board_number_1[row][col] = temp;
        std::string s = t ? "Chicken" : "Bunny";
        //int temp = board_number_1[row][col];

        //////////////-----------Vertical check
        int right_half = 0;
        int left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (row + i > 8 || board_number_1[row + i][col] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (row - i < 0 || board_number_1[row - i][col] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            label_1->setString(s + " has collected all the eggs!");
            menu_1->setVisible(true);
            return true;
        }
        //if (right_half + left_half >= 4) Director::getInstance()->stopAnimation();

        ///////////----------------Horizontal check
        right_half = 0;
        left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (col + i > 8 || board_number_1[row][col + i] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (col - i < 0 || board_number_1[row][col - i] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            label_1->setString(s + " has collected all the eggs!");
            menu_1->setVisible(true);
            return true;
        }
        //if (right_half + left_half >= 4) Director::getInstance()->pause();

        ///////////------------Main cross check
        right_half = 0;
        left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (col + i > 8 || row + i > 8 || board_number_1[row + i][col + i] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (col - i < 0 || row - i < 0 || board_number_1[row - i][col - i] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            label_1->setString(s + " has collected all the eggs!");
            menu_1->setVisible(true);
            return true;
        }

        ///////////------------Secondary cross check
        right_half = 0;
        left_half = 0;
        for (int i = 1; i < 5; i++) {
            if (col + i > 8 || row - i < 0 || board_number_1[row - i][col + i] != temp) break;
            else right_half++;
        }
        for (int i = 1; i < 5; i++) {
            if (col - i < 0 || row + i > 8 || board_number_1[row + i][col - i] != temp) break;
            else left_half++;
        }
        if (right_half + left_half >= 4) {
            label_1->setString(s + " has collected all the eggs!");
            menu_1->setVisible(true);
            return true;
        }
    };

    auto fireEvent = [](int pos_x, int pos_y) -> void {
        auto touch = Touch::Touch();
        touch.setTouchInfo(1, pos_x, pos_y);
        std::vector<Touch*> v;
        v.push_back(&touch);

        auto eventTouch = EventTouch::EventTouch();
        eventTouch.setEventCode(EventTouch::EventCode::ENDED);
        eventTouch.setTouches(v);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&eventTouch);
        //touch.release();
        //eventTouch.release();
    };
    
    auto randommove = []() -> std::vector<int> {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board_number_1[i][j] == 0) {
                    std::vector<int> v;
                    v.push_back(i);
                    v.push_back(j);
                    return v;
                }
            }
        }
    };
    //////////////////////////// 9X9 BUTTON
    start_pos_x = origin.x + visibleSize.width / 2 - size_of_caro * 4;
    start_pos_y = origin.y + visibleSize.height / 2 + size_of_caro * 4;

    /*
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            auto button = ui::Button::create();
                        button->loadTextureNormal("normal.png");
            button->setOpacity(0);
            button->setTitleText("");
            button->setPosition(Vec2(start_pos_x + size_of_caro * j, start_pos_y));
            button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchHandler, this, i, j));
            this->addChild(button, 1);
        }
        start_pos_y -= size_of_caro;
    }
    */
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            auto button = ui::Button::create();
            button->loadTextureNormal("normal.png");
            button->setOpacity(0);
            button->setTitleText("");
            button->setPosition(Vec2(start_pos_x + size_of_caro * j, start_pos_y));
            button->addTouchEventListener([i, j, checkWinner, fireEvent, randommove](Ref* sender, ui::Widget::TouchEventType type) {
                auto temp = dynamic_cast<ui::Button*>(sender);
                if (type == ui::Widget::TouchEventType::ENDED)
                {
                    //logic win/lose, turn_1 around, send location
                    if(turn_1) temp->loadTextureDisabled("chick.png");
                    else temp->loadTextureDisabled("bunny.png");
                    temp->getRendererDisabled()->setOpacity(255);
                    //temp->loadTextureDisabled("bunny.png");
                    temp->setEnabled(false);
                    //text_label_1 << i << "," << j << ";";
                    temp->setTitleColor(Color3B::BLACK);
                    if (checkWinner(i, j, turn_1)) return;
                    turn_1 = !turn_1;
                    //fireEvent(caro_position_1[4][4].x, caro_position_1[4][4].y);
                    ////////////////////////-----------------AI
                    
                    std::vector<int> move = randommove();

                    
                    temp = button_grid_1[(move[0] * 9) + (move[1] + 1) - 1];
                    //temp = button_grid_1[4];
                    if (turn_1) temp->loadTextureDisabled("chick.png");
                    else temp->loadTextureDisabled("bunny.png");
                    temp->getRendererDisabled()->setOpacity(255);
                    //temp->loadTextureDisabled("bunny.png");
                    temp->setEnabled(false);
                    //text_label_1 << i << "," << j << ";";
                    temp->setTitleColor(Color3B::BLACK);
                    checkWinner(move[0], move[1], turn_1);
                    turn_1 = !turn_1;

                    //std::cout << "Button 1 clicked" << std::endl;
                }
            });
            this->addChild(button, 1);
            button_grid_1.push_back(button);
        }
        start_pos_y -= size_of_caro;
    }
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto end = MenuItemImage::create("item.png", "item.png", CC_CALLBACK_1(HelloWorld::newGame, this));
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
    menu_1 = Menu::create(end, NULL);
    menu_1->setPosition(Vec2::ZERO);
    this->addChild(menu_1, 2);
    menu_1->setVisible(false);
    /*
    /////////////////////////////
    // 3. add your codes below...

    // add a label_1 shows "Hello World"
    // create and initialize a label_1


    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    return_1 true;
    */
    ////////////// BOARD
    auto grid = Sprite::create("grid_update.png");
    if(grid == nullptr)
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


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    auto pause = Pause::createScene();
    Director::getInstance()->pushScene(pause);

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::newGame(cocos2d::Ref* s) {
    auto newgame = End::createScene();
    Director::getInstance()->replaceScene(newgame);
    // return;
}