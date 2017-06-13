// eng_message.hpp

#pragma once

//***************************************
// Class: MessageBase *******************
//***************************************
// Description:
//  Base class for messages
//***************************************

typedef uint8_t msgtype;

enum MsgType{MSGTYPE_BASIC, MSGTYPE_RENDER};

class MessageBase{
    msgtype message_type;
public:
    MessageBase() : message_type(MSGTYPE_BASIC){}
};
