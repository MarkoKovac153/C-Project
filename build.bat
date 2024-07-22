set files=src\glad.c src\main.c src\engine\global.c src\engine\render\render.c src\engine\render\render_init.c
set libs=C:\Users\marko\Desktop\personalStuff\Github\cproject\lib\SDL2main.lib C:\Users\marko\Desktop\personalStuff\Github\cproject\lib\SDL2.lib C:\Users\marko\Desktop\personalStuff\Github\cproject\lib\freetype.lib

CL /ZI /I C:\Users\marko\Desktop\personalStuff\Github\cproject\include %files% /link %libs% /OUT:mygame.exe