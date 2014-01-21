
/**
 * This key map file is for some device whose key code is not define in linux/input.h
 * but is needed by recovery.
 * For example : IR
 *
 * If you add/modify some keys , pls notice function where these keys are used.
 * CheckKey(int key)  HandleMenuKey(int key, int visible)
 */
#define IR_KEY_POWER            538
#define IR_KEY_UP               596
#define IR_KEY_LEFT             580     //key 580    DPAD_LEFT       WAKE_DROPPED
#define IR_KEY_RIGHT            604     //key 604    DPAD_RIGHT      WAKE_DROPPED
#define IR_KEY_DOWN             592
#define IR_KEY_ENTER            588
#define IR_KEY_MUTE             542
#define IR_KEY_VOLUME_UP        518
#define IR_KEY_VOLUME_DOWN      514
#define IR_KEY_CHANNEL_UP       534
#define IR_KEY_CHANNEL_DOWN     530
#define IR_KEY_QUIT             522
#define IR_KEY_BACK             526
#define IR_KEY_PAGE_UP          576
#define IR_KEY_PAGE_DOWN        584
#define IR_KEY_GUID             539
#define IR_KEY_VOD              535
#define IR_KEY_SERVICE          515
#define IR_KEY_MENU             523
#define IR_KEY_FAST_REVRSE      581     //key 581    FAST_REVERSE
#define IR_KEY_PALY             589     //key 589    PLAY
#define IR_KEY_STOP             597     //key 597    STOP
#define IR_KEY_FAST_FORWARD     605     //key 605    FAST_FORWARD
#define IR_KEY_REFRESH          577     //key 577    REFRESH
#define IR_KEY_TV               527     //key 527   TV
#define IR_KEY_MUSIC            531     //key 531   MUSIC
#define IR_KEY_SD               545     //key 545    SD
#define IR_KEY_HD               546     //key 546    HD
#define IR_KEY_FAVORITE         543     //key 543    FAVORITE
#define IR_KEY_CONFIG           519     //key 519    CONFIG
#define IR_KEY_SEARCH           512     //key 512    SEARCH
#define IR_KEY_HELP             600     //key 600    HELP
#define IR_KEY_RECORD           520     //key 520    FAST_RECORD
#define IR_KEY_SOUND_EFFECT     517     //key 517    SOUND_EFFECT
#define IR_KEY_ENTER            588     //key 588    ENTER
#define IR_KEY_STAR             524     //key 524    STAR
#define IR_KEY_POUND            540     //key 540    POUND
#define IR_KEY_1                585     //key 585    1
#define IR_KEY_2                593     //key 593    2
#define IR_KEY_3                601     //key 601    3
#define IR_KEY_4                525     //key 525    4
#define IR_KEY_5                533     //key 533    5
#define IR_KEY_6                541     //key 541    6
#define IR_KEY_7                521     //key 521    7
#define IR_KEY_8                529     //key 529    8
#define IR_KEY_9                537     //key 537    9
#define IR_KEY_0                532     //key 532    0
