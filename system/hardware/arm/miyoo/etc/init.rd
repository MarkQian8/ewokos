/bin/ipcserv /drivers/miyoo/gpio_joykeybd      /dev/joykeyb
#/bin/ipcserv /drivers/miyoo/gpio_joykeyb_minid /dev/joykeyb
#/bin/ipcserv /drivers/miyoo/gpio_joystickd     /dev/joystick
/bin/ipcserv /drivers/miyoo/audctrl            /dev/sound

/bin/ipcserv /drivers/miyoo/fbd                /dev/fb0
/bin/ipcserv /drivers/displayd       /dev/display /dev/fb0
/bin/ipcserv /drivers/fontd          /dev/font /usr/system/fonts/system.ttf
/bin/ipcserv /drivers/consoled       /dev/console0

/bin/ipcserv /drivers/timerd         /dev/timer
/bin/ipcserv /drivers/nulld          /dev/null
/bin/ipcserv /drivers/ramfsd         /tmp
/bin/ipcserv /drivers/proc/sysinfod  /proc/sysinfo
/bin/ipcserv /drivers/proc/stated    /proc/state

@/sbin/sessiond &
@/bin/session &

/bin/ipcserv /drivers/xserverd       /dev/x
@/sbin/x/xim_none   /dev/joykeyb &
#@/sbin/x/xjoystickd /dev/joystick &
@/sbin/x/xim_vkey 560 160&
@/bin/x/xsession &
