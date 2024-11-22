#ifndef MAIN_H
#define MAIN_H

   //---------------------------------------------------------------------------//
  //-----------------------------ENUMBS----------------------------------------//
 //---------------------------------------------------------------------------//

// Gate keeper state mashine
 enum KEEPER_STATE {
    STATE_INIT,
    STATE_INIT_FAIL,
    STATE_INIT_SUCESS,
 };

 //gate keeper state, start on initialization
KEEPER_STATE gateKeeperState = STATE_INIT;

#endif