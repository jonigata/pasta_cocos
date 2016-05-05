#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "board.hpp"
#include "player.hpp"
#include "ai.hpp"
#include "LifeBarLayer.h"

class BattleScene : public cocos2d::Layer
{
  public:
    static cocos2d::Scene* createScene();

    BattleScene() : player_(board_), ai_(board_) {}

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(BattleScene);

    void update(float delta) override;

  private:
    Board board_;
    Player player_;
    Ai ai_;
    LifeBarLayer* lifeBarLayer_;

};

#endif // __HELLOWORLD_SCENE_H__
