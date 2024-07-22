set files=src\glad.c src\main.c
set libs=C:\Users\marko\Desktop\personalStuff\Github\cproject\lib\SDL2main.lib C:\Users\marko\Desktop\personalStuff\Github\cproject\lib\SDL2.lib C:\Users\marko\Desktop\personalStuff\Github\cproject\lib\freetype.lib

CL /ZI /I C:\Users\marko\Desktop\personalStuff\Github\cproject\include %files% /link %libs% /OUT:mygame.exe