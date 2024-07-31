set render=src\engine\render\render.c src\engine\render\render_init.c src\engine\render\render_util.c 
set io=src\engine\io\io.c 
set files=src\glad.c src\main.c src\engine\global.c %render% %io%
set libs=C:\Users\marko\Desktop\Projects\C-Project\lib\SDL2main.lib C:\Users\marko\Desktop\Projects\C-Project\lib\SDL2.lib C:\Users\marko\Desktop\Projects\C-Project\lib\freetype.lib

CL /ZI /I C:\Users\marko\Desktop\Projects\C-Project\include %files% /link %libs% /OUT:mygame.exe