#include "cedit.h"

void dialog_savefile(){cmd_new("save file:", "savefile");}
void dialog_savedfile(){draw_info("file saved");}
void dialog_openfile(){cmd_new("open file:", "openfile");}
void dialog_openedfile(){draw_info("file opened");}
