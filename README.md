# SteamLibrarian
A tool to move applications between Steam libraries.

Steam supports storing applications in different locations (called 'libraries'), but it doesn't offer a built-in way
to move applications between different libraries. Although moving applications manually is possible and not very hard
once you know the steps, SteamLibrarian provides a convenient and automatic way.

Contemporary computers typically have both an SSD and a classic hard disk drive, with the SSD fast but small and the
hard disk large but relatively slow.
While all applications generally benefit from the speed of an SSD, some games especially benefit, as they typically
load large amounts of data when starting or changing a level and/or "stream" additional data while playing. 
However, the SSD usually contains the operating system as well as installed applications, which increases the
"disk space pressure", especially if you wish to install multiple large games at once.

In such a scenario SteamLibrarian can help you to organize your games between the SSD and HDD, and allows you to
move games around as you play new games, put old ones aside or change your favorites.

Requirements
------------
* OS: Windows Vista and newer (tested on Windows 7 and 8.1)
* Doesn't make much sense without Steam

Building from source
--------------------
* Developed with Visual Studio 2013
* Dependencies: Qt 5.4, Boost 1.57
* Requires CMake, WiX Toolset for building

Contact
-------
You can reach me by mail: frank.richter@gmail.com
