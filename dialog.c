#include "cedit.h"

void dialog_savefile()            {cmd_new("save file:", "savefile", 0);}
void dialog_savefile_success()    {draw_info("file saved");}
void dialog_savefile_failure()    {draw_info("error saving file");}

void dialog_openfile()            {cmd_new("open file:", "openfile", 0);}
void dialog_openfile_success()    {draw_info("file opened");}
void dialog_openfile_failure()    {draw_info("error opening file");}

