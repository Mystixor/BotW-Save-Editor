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

In case you encounter any problems, please notify me via Discord, you can find me on the BotW Modding Discord (Mystixor#6773).

# Troubleshooting
1.  A voluntary tester could not launch the EXE until he installed mingw32, I'm not sure what it was about but I will try to build with VS2017 in the future to avoid compiler problems.
2.  At some point the tool did not want to work for me when I had the save file on another drive, so maybe try copying the save file over.

# License stuff
You may use my code in any way you wish, as long as I'm openly credited ;)
