#include "ftp.hpp"
#include "ftp\ftpserver.h"


FtpServer ftpSrv;

void initFtp()
{
    ftpSrv.init();
}

void handleFtp()
{
    ftpSrv.service();
}