#/bin/ipcserv /drivers/raspix/soundd   /dev/sound
/bin/ipcserv /drivers/raspix/usbd     /dev/touch0

/bin/ipcserv /drivers/raspix/fbd      /dev/fb0
/bin/ipcserv /drivers/displayd        /dev/display /dev/fb0
#/bin/ipcserv /drivers/fontd           /dev/font /usr/system/fonts/system.ttf /usr/system/fonts/system_cn.ttf
/bin/ipcserv /drivers/fontd           /dev/font /usr/system/fonts/system.ttf
/bin/ipcserv /drivers/consoled        /dev/console0

/bin/ipcserv /drivers/timerd          /dev/timer
/bin/ipcserv /drivers/ramfsd          /tmp

/bin/ipcserv /drivers/nulld           /dev/null
/bin/ipcserv /drivers/proc/sysinfod   /proc/sysinfo
/bin/ipcserv /drivers/proc/stated     /proc/state

@/sbin/sessiond &
@/bin/session /bin/tsaver &

/bin/ipcserv /drivers/xserverd        /dev/x
#@/sbin/x/xmoused /dev/mouse0 &
@/sbin/x/xim_none &
@/sbin/x/xim_vkey 560 168&

@/bin/x/xsession  &
