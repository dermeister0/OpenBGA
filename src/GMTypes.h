//Game Message Types
//“ипы сообщений

#define BGA_MSG_NEW_LOCAL_PLAYER 1 //создание всех локальных игроков в начале игры
#define BGA_MSG_EXIST_REMOTE_PLAYER 2 //удаленный игрок, существующий к началу подключени€
#define BGA_MSG_NEW_REMOTE_PLAYER 3 //новый подключившийс€ игрок
#define BGA_MSG_END_OF_PLAYERS_LIST 4 //конец списка игроков

#define BGA_MSG_WORLD_INFO 5 //обща€ информаци€ (название карты,...) дл€ загрузки/перезагрузки мира

#define BGA_MSG_UPDATE_PLAYER 6 //обеновл€ет позицию и состо€ние каждого игрока
#define BGA_MSG_UPDATE_FRAGS 7 //обновл€ет фраги дл€ уазанного игрока, а также макс. фраги

#define BGA_MSG_NEW_BULLET 8 //создает пулю
#define BGA_MSG_NEW_EXPLODE 9 //создает взрыв

#define BGA_MSG_PLAY_SOUND 10 //проигрывает звук