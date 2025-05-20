#define RELAX "http://icecast7.play.cz/relax128.mp3"
#define ROCK "http://ice.radia.cz/rockzabava128.mp3"
#define EXPRESS_SK "http://stream.bauermedia.sk/expres-lo.mp3"
#define DANCE "http://ice.actve.net/dance-radio128.mp3"
#define VLTAVA "http://icecast3.play.cz/cro3-64.mp3"
#define KISS "http://n22a-eu.rcs.revma.com/asn0cmvb938uv?rj-ttl=5&rj-tok=AAABlu10mMsAnsEg4VXHd5VSeA"
#define KOSICE "http://stream.21.sk/radiokosice-128.mp3"
#define BUDAPEST "http://icast.connectmedia.hu/5202/live.mp3"
#define KOSSUTH "http://mr-stream.connectmedia.hu/4736/mr1.mp3"
#define DEUTSCH_ROCK "http://germanyrock.stream.laut.fm/germanyrock?t302=2025-05-20_11-41-30&uuid=d810e070-ba58-4b2f-ae8f-8fb2fb14321d"
#define FRANCE_CULTURE "http://direct.franceculture.fr/live/franceculture-lofi.mp3"
#define ITALY_MUSIC "http://italymusic.stream.laut.fm/italymusic?t302=2025-05-20_11-43-26&uuid=34325985-fa9b-4269-ae23-5d8ebc8bd1b5"
#define JAPAN_HITS "http://cast1.torontocast.com:2120/;"
#define NETWORK "http://10.42.0.1:8000/stream.mp3"


static char* stations[] = {
    RELAX, ROCK, EXPRESS_SK, DANCE, VLTAVA, DEUTSCH_ROCK, KOSSUTH, BUDAPEST, KOSICE, FRANCE_CULTURE, ITALY_MUSIC, JAPAN_HITS, NETWORK
};

#define RADIO_COUNT (sizeof(stations) / sizeof(char*))