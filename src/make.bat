path=c:\devkitadv\bin

gcc  -o sc2gba.elf sc.cpp bg.cpp ships.cpp status.cpp fire.cpp special.cpp set.cpp melee.cpp choose.cpp restore.cpp pkunk.cpp urquan.cpp -lm 

objcopy -O binary sc2gba.elf sc2gba.bin
