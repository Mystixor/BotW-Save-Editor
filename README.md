# BotW Save Editor
This is my Save Editor for BotW (Wii U) written in C++.
Big thanks to zephenryus, assisting to reverse engineer the save files.

# How to use:
1.  Drag your "\[game_data/caption/option\].sav" onto "Simple Save Editor.exe" and let it work for a moment.
2.  Open the created "\[game_data/caption/option\].sav.json" in any text editor.<br />
  a)  Editing "DataName" will not have any effect at all.<br />
  b)  Editing "HashValue" will break the save file.<br />
  c)  Editing "DataValue" works, pay attention that it is always written as an Integer.
3.  Save and drag the JSON onto the EXE again.
4.  Have fun with the newly created save file!

In case you encounter any problems, please notify me via Discord, you can find me on the BotW Modding Discord (Mystixor#6773).

# Disclaimer
This project was very old and I randomly decided to fix some old issues of which I now have a much better understanding than 6 years ago. Instead I have much less idea of what this was supposed to be doing back then. I did some testing after fixing up the code and bringing it closer to my modern standards, but it's not impossible I introduced new incorrect behavior.

# License stuff
You may use my code in any way you wish, as long as I'm openly credited ;)
