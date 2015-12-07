#include "HelloWorldScene.h"
#include "controller.h"

USING_NS_CC;
using namespace ui;

Scene* HelloWorld::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = HelloWorld::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool HelloWorld::init() {
  if(!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto editBox = EditBox::create(Size(visibleSize.width,visibleSize.height*0.5f),Scale9Sprite::create("HelloWorld.png"));
  editBox->setFont("font/arial.ttf",46.0f);
  editBox->setPlaceHolder("URL");
  editBox->setPlaceholderFontColor(Color3B::BLACK);
  editBox->setFontColor(Color3B::BLACK);
  editBox->setMaxLength(100);
  editBox->setText("ws://localhost:7681/input");
  editBox->setReturnType(EditBox::KeyboardReturnType::DONE);
  editBox->setInputMode(EditBox::InputMode::ANY);
  editBox->setDelegate(this);
  editBox->setPosition(Size(visibleSize.width*0.5f,visibleSize.height*0.25 * 3));
  this->addChild(editBox);

  auto mItem1 = MenuItemImage::create("CloseNormal.png","CloseNormal.png",[=](Ref*sender) {
    Controller::getURL(editBox->getText());
    Controller::getPlayerID(0);
    Director::getInstance()->replaceScene(Controller::createScene());
  });
  auto mItem2 = MenuItemImage::create("CloseSelected.png","CloseSelected.png",[=](Ref*sender) {
    Controller::getURL(editBox->getText());
    Controller::getPlayerID(1);
    Director::getInstance()->replaceScene(Controller::createScene());
  });

  mItem1->setPosition(Point(visibleSize.width*0.25f*1,visibleSize.height*0.25 * 1));

  Vec2 scl = visibleSize*0.5f;
  mItem1->setScale(scl.x / mItem1->getContentSize().width,scl.y / mItem1->getContentSize().height);
  mItem2->setScale(scl.x / mItem1->getContentSize().width,scl.y / mItem1->getContentSize().height);
  mItem2->setPosition(Point(visibleSize.width*0.25f*3,visibleSize.height*0.25 * 1));
  
  //ƒƒjƒ…[‚ğì¬
  auto menu = Menu::create(mItem1,mItem2,NULL);
  menu->setPosition(Point::ZERO);
  this->addChild(menu);

  return true;
}
