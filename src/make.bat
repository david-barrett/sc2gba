gcc  -o sc.elf sc.cpp bg.cpp ships.cpp status.cpp fire.cpp special.cpp set.cpp game.cpp choose.cpp -lm

objcopy -O binary sc.elf sc.bin
