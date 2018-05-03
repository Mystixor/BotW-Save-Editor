# *.sav format
0x00  version indicator (int)

0x04  placeholder, always 0xFFFFFFFF

0x08  maybe format version? always 0x00000001

0x0C  start of save data array:

  0x00  hash value (int)
  
  0x04  data value (int, uint, char, etc.)
  


The hash value is found again in Bootup.pack --> GameData/savedataformat.ssarc
There every hash is assigned a readable name that helps understanding what the data value defines.
