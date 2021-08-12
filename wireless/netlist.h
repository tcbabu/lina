typedef struct _net_list {
  int id;
  int sig;
  int sec;
  char ssid[200];
  char bssid[50];
  int status; //0 disabled 1 enabled 2 current
  char psk[200];
  char uname[100]; // presently not used
  char passwd[100];// presently not used
  int proto; // 1 WPA 2 WPA2
  int group; // 1 TKIP 2 CCMP
} NETLIST;
