# ShipLog (RELEASE 1)
This plugin lets you see stdio on the vita


//Put together by dots_tb

//Created for Kancolle Kai Vita translation and dev team (expecially you senpai ~<3)

//Special thanks to Team_Molecule for Taihen (special thanks to xyz)

//thanks to xerpi for being underrated (and logging functions/netdebug), frangarcj for oclock

//Freakler for common dialog, TheFlow for VitaShell

## To-Do
 * usb
 * others
 * exports
## Installation
Add the library to your "*KERNEL" section in the TaiHen config (ux0:/tai/config.txt)

  ux0:tai/backdoor_exe.skprx

Add the the user plugin to a game in the TaiHen config (ux0:/tai/config.txt)

  *titleid

  ux0:tai/shipLog.suprx

Move the net_plugin to the data directory:

  ux0:data/shipLog/shipLog_net.suprx

## Usage
It will create logs at "ux0:/dump/*"

This is because it uses xerpi's code.

To use net, enable the plugin for a game.

Run netcat:

  nc -l -p 3333

Set up Ip and Port in WarRoom/Configuration app

The whole buffer will be redirected to netcat when game is running.


## READ ME

I am not responsible for anything that happens to your vita.

## Further Thanks:
 * xyz - sceIoWrite tip
 * #vitasdk - putting up with stupid questions (I was that guy..)
 * yasen - bringing me on the dev* team
 * coburn64 - unity engine help
 * xepri - LOGging from that one code snippet
 * frangarcj for oclock, Freakler for common dialog, TheFlow for VitaShell
 * everyone on the kancolle kai vita discord
 * thank you yifan lu for trying to help
