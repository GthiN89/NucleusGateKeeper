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



enum CC1101_PRESET {
     AM650,
     AM270,
     FM238,
     FM476,
     FM95,
     FSK12k,
     FM15k,
     FSK25k,
     FSK31k,
     PAGER,
     HND1,
     HND2,
     CUSTOM
 };
extern CC1101_PRESET  C1101preset;

#endif