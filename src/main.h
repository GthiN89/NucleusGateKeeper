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
   STATE_SDCARD_EROOR,
   STATE_IDLE,
   STATE_CAPTURE,
   STATE_CAPTURE_COMPLETE,
   STATE_SIGNAL_ANALYZER,
   STATE_SIGNAL_ANALYZER_COMPLETE,
   STATE_FILE_OPERATION,
   STATE_READY_TO_ACTION
 };

 //gate keeper state, start on initialization
extern KEEPER_STATE gateKeeperState;

#endif