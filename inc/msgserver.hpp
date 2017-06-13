// msgserver.hpp - Message server

#pragma once

#include "common.hpp"
#include "eng_message.hpp"

//***************************************
// Class: SubsystemBase (abstract) ******
//***************************************
// Description:
//  Base class for all engine subsystems
//  Implements an interface for GetMessage method
//***************************************

class SubsystemBase{
    bool valid;
public:
    SubsystemBase() : valid(true) {};
    virtual void GetMessage(MessageBase* msg) = 0;
};

//***************************************
// Class: MsgServer *********************
//***************************************
// Description:
//  Global Message Server class. Hold pointers
//  for all valid engine subsystems that should
//  receive messages, and reroutes those messages
//  to them
//***************************************

class MsgServer{
    std::vector<SubsystemBase*> subsystems;
public:
    MsgServer(){};
    void RegisterSubSystem(SubsystemBase *sub);
    void SendMessage(MessageBase* msg);
};

extern MsgServer msgserver;
