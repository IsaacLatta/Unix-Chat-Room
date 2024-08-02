#include "chatroom.h"

#define PORT 5555

int main()
{
    ChatRoom chatroom;
    init_chatroom(&chatroom, PORT);
    run_chatroom(&chatroom);
    
    return 0;
}