
/**
 * @file   builder.cpp
 * @Author Adam Piecek
 * @date   May, 9
 *
 */

#include "builder.h"


void Builder::build(){
    cs.size = size;
    size_t* sizePtr = &cs.size;
    cs.ai = ai;
    msg = std::unique_ptr  <Messenger>					(new Messenger(&cs));
    gsm = std::unique_ptr  <GameStateManipulator>		(new GameStateManipulator(msg.get()));
    scf = std::unique_ptr  <Scout_flip>					(new Scout_flip(msg.get(), sizePtr));
    scg = std::unique_ptr  <Scout_grey>					(new Scout_grey(msg.get(), sizePtr));
    fl  = std::unique_ptr  <Flipper>					(new Flipper(msg.get(), scf.get()));

    bh  = std::unique_ptr  <Board_History>				(new Board_History(msg.get(), sizePtr));
    sh  = std::unique_ptr  <Score_History>				(new Score_History(msg.get()));
    th  = std::unique_ptr  <Turn_History>				(new Turn_History(msg.get()));
    iog = std::unique_ptr  <IO_Game>					(new IO_Game(msg.get()));
    Builder::build_AI();
}

void Builder::build_AI(){
    size_t* sizePtr = &cs.size;
    if(ai == othAi::noneAi){
        ms  = std::unique_ptr  <Move_switcher>			(new Move_switcher(msg.get(), ai, &cs));
        gm  = std::unique_ptr  <Grey_Manager>			(new Grey_Manager(msg.get(), scg.get(), sizePtr));
        msg->init(drawCallback, gsm.get(), ms.get(), th.get(), fl.get(), scf.get(), scg.get(), gm.get(), sh.get(), bh.get(), NULL, iog.get());
        msg->load_newPtr(gm.get(), NULL);
    }
    else if(ai == othAi::firstAi){
        ms  = std::unique_ptr  <Move_switcher>			(new Move_switcher(msg.get(), ai, &cs));
        gmai  = std::unique_ptr  <Grey_Manager_ForAI>	(new Grey_Manager_ForAI(msg.get(), scg.get(), sizePtr));
        // pridane AI_1
        ai1 = std::unique_ptr	<AI_1>					(new AI_1(msg.get(), get_GameState(), scf.get()));
        msg->init(drawCallback, gsm.get(), ms.get(), th.get(), fl.get(), scf.get(), scg.get(), gmai.get(), sh.get(), bh.get(),
        // ->	  v-v doplnit do konstruktoru
                  ai1.get(), iog.get());
        msg->load_newPtr(gmai.get(), ai1.get());
    }
    else if(ai == othAi::secondAi){
        ms  = std::unique_ptr  <Move_switcher>			(new Move_switcher(msg.get(), ai, &cs));
        gmai  = std::unique_ptr  <Grey_Manager_ForAI>	(new Grey_Manager_ForAI(msg.get(), scg.get(), sizePtr));
        // pridane AI_2
        ai2 = std::unique_ptr	<AI_2>					(new AI_2(msg.get(), get_GameState(), scf.get()));
        msg->init(drawCallback, gsm.get(), ms.get(), th.get(), fl.get(), scf.get(), scg.get(), gmai.get(), sh.get(), bh.get(),
        // ->	  v-v doplnit do konstruktoru
                  ai2.get(), iog.get());
        msg->load_newPtr(gmai.get(), ai2.get());
    }
    /*
    else if(ai == othAi::thirdAi){
        msgai = std::unique_ptr  <Messenger_ForAI>		(new Messenger_ForAI(&csai));
        // pridane AI_2
        ai2 = std::unique_ptr	 <AI_2>					(new AI_2(msgai.get(), get_GameState()));

        gmai  = std::unique_ptr  <Grey_Manager_ForAI>	(new Grey_Manager_ForAI(msg.get(), scg.get(), cs.size));
        // Move_switcher pro potrebu AI
        msai  = std::unique_ptr  <Move_switcher>		(new Move_switcher(msg.get(), othAi::noneAi, &csai));
        thai  = std::unique_ptr  <Turn_History>			(new Turn_History(msgai.get()));
        shai  = std::unique_ptr  <Score_History>		(new Score_History(msgai.get()));
        bhai  = std::unique_ptr  <Board_History>		(new Board_History(msgai.get(), cs.size));
        msgai->initai(msai.get(), th.get(), thai.get(), fl.get(), scf.get(),
                    scg.get(), gmai.get(), sh.get(), shai.get(), bh.get(), bhai.get());

        msg->init(drawCallback, gsm.get(), ms.get(), th.get(), fl.get(), scf.get(), scg.get(), gm.get(), sh.get(), bh.get(),
        // ->	  v-v doplnit do konstruktoru
                  ai2.get());
    }
    */

}

int Builder::loadgame(std::string name){
    fromfile* ffptr = iog->loadGame(name);
    cs.ai = ffptr->ai;
    cs.size = ffptr->size;
    if(ai == othAi::noneAi){
        ms.release();
        gm.release();
    }
    else if(ai == othAi::firstAi){
        ms.release();
        gmai.release();
        ai1.release();
    }
    else if(ai == othAi::secondAi){
        ms.release();
        gmai.release();
        ai2.release();
    }
    build_AI();
    return cs.size;
}
