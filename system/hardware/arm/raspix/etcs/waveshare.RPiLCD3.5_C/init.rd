/bin/ipcserv /drivers/raspix/rpi_lcdd      /dev/fb0
/bin/ipcserv /drivers/displayd             /dev/display /dev/fb0
/bin/ipcserv /drivers/fontd                /dev/font /usr/system/fonts/system.ttf
/bin/ipcserv /drivers/consoled             /dev/console0

/bin/ipcserv /drivers/timerd               /dev/timer
/bin/ipcserv /drivers/nulld                /dev/null
/bin/ipcserv /drivers/ramfsd               /tmp
/bin/ipcserv /drivers/proc/sysinfod        /proc/sysinfo
/bin/ipcserv /drivers/proc/stated          /proc/state

/bin/ipcserv /drivers/xserverd             /dev/x

@/sbin/sessiond &
@/bin/session &

/bin/ipcserv /drivers/xserverd             /dev/x
@/sbin/x/xtouchd &
@/sbin/x/xim_vkey &

@/bin/x/xsession &