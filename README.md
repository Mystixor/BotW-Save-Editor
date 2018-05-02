# BotW Save Editor
This is my Save Editor for BotW (Wii U) written in C++.
Big thanks to zephenryus, assisting to reverse engineer the save files.

# How to use:
1.  Drag your "\[game_data/caption/option\].sav" onto "Simple Save Editor.exe" and let it work for a moment.
2.  Open the created "\[game_data/caption/option\].sav.json" in any text editor.
  a)  Editing "DataName" will not have any effect at all.
  b)  Editing "HashValue" will break the save file.
  c)  Editing "DataValue" works, pay attention that it is always written as an Integer.
3.  Save and drag the JSON onto the EXE again.
4.  Have fun with the newly created save file!

# License stuff
You may use my code in any way you wish, as long as I'm openly credited ;)
