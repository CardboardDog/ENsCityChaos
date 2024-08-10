#include "arena.hpp"
using namespace bn;
#define curtainSlice(x,y) sprite_items::curtain.create_sprite(x*64,y*64)
#define curtainLayer(y) curtainSlice(0,y),curtainSlice(1,y),curtainSlice(-1,y),curtainSlice(2,y),curtainSlice(-2,y)
namespace arena{
    fixed tangent(int theta){ 
        /* 
            This was originally a pretty fast
            three-liner, but somehow it grew 
            out of control. It's not really
            the best choice when you need
            speed, but it has more features
            than my original implementation, 
            making it more versatile. oh well.
            (sorry if this function makes you
            want to bleach your eyeballs)
        */
        pair<fixed,fixed> sin_cos;
        if(theta<0)
            sin_cos = degrees_lut_sin_and_cos(360-(abs(theta)%360));
        else
            sin_cos = degrees_lut_sin_and_cos(theta%360);
        if(sin_cos.second==0)return (sin_cos.first<0)?9999:-9999; // good enough :/
        if(theta>=0)return sin_cos.first/sin_cos.second;
        return sin_cos.first/sin_cos.second;
    }
    void battle(){
        regular_bg_ptr sky = regular_bg_items::mountains.create_bg(0,0);
        switch(stages::picked){
            case 0:
                sky.set_item(regular_bg_items::mountains);
                break;
            case 1:
                sky.set_item(regular_bg_items::foliage);
                break;
            case 2:
                sky.set_item(regular_bg_items::space);
                break;
            case 3:
                sky.set_item(regular_bg_items::night);
                break;
            default:
                stages::picked=0;
                break;
        }
        sprite_ptr ptrPile[4] = {
            sprite_items::boxer.create_sprite((0-1.5)*32,20),
            sprite_items::null.create_sprite((1-1.5)*32,20),
            sprite_items::null.create_sprite(((2-1.5)*32),20),
            sprite_items::null.create_sprite((3-1.5)*32,20)
        };
        for(int i=0;i<4;i++){
            ptrPile[i].set_visible(false);
        }
        players::player* allPlayers[4] = {
            new players::player(players::picked[0],&ptrPile[0]),
            new players::player(players::picked[1],&ptrPile[1]),
            new players::player(players::picked[2],&ptrPile[2]),
            new players::player(players::picked[3],&ptrPile[3])
        };
        switch(stages::picked){
            case 0:
                allPlayers[0]->setRespawn(40,40);
                allPlayers[1]->setRespawn(-20,39);
                allPlayers[2]->setRespawn(112,20);
                allPlayers[3]->setRespawn(-105,-1);
                break;
            case 1:
                allPlayers[0]->setRespawn(40,0);
                allPlayers[1]->setRespawn(-40,0);
                allPlayers[2]->setRespawn(15,-25);
                allPlayers[3]->setRespawn(-15,-24);
                break;
            case 2:
                allPlayers[0]->setRespawn(0,37);
                allPlayers[1]->setRespawn(-55,-21);
                allPlayers[2]->setRespawn(55,-21);
                allPlayers[3]->setRespawn(0,-57);
                break;
            case 3:
                allPlayers[0]->setRespawn(10,21);
                allPlayers[1]->setRespawn(-30,21);
                allPlayers[2]->setRespawn(80,-2);
                allPlayers[3]->setRespawn(-60,-29);
                break;
            default:
                allPlayers[0]->setRespawn(0,0);
                allPlayers[1]->setRespawn(0,0);
                allPlayers[2]->setRespawn(0,0);
                allPlayers[3]->setRespawn(0,0);
                return;
                break;
        }
        camera_ptr curtainCam = camera_ptr::create(0,0);
        sprite_ptr sprites[] = {
            curtainLayer(-1),
            curtainLayer(0),
            curtainLayer(1)
        };
        for(int i=0;i<15;i++)sprites[i].set_camera(curtainCam);
        int animFrame = 0;
        while(1){
            curtainCam.set_position(0,animFrame);
            animFrame+=3;
            core::update();
            if(animFrame>210){
                break;
            }
        }
        animFrame=-50;
        sprite_ptr countDown = sprite_items::count.create_sprite(0,0);
        sprite_animate_action<3> countAnim = create_sprite_animate_action_forever(countDown,90,sprite_items::count.tiles_item(),2,1,0);
        while(animFrame<225){
            countDown.set_position(tangent(animFrame*2)*50,0);
            countAnim.update();
            core::update();
            animFrame++;
        }
        while(1){
            core::update();
        }
    }
}