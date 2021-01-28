# Implementations

This is the README file for the implementations of XMusic. If you're here,
you are either curious to learn a bit about the implementations or 
you want to make your own. Implementations are basically pieces of code
that XMusic uses behind the scenes to do all the required work (like
play audio, handle graphics, files, input etc.). To make your own, you
need to follow this format in the file names:

`implementation_*ware_library.h` (*ware in this case referring to
how it draws its graphics and/or handles audio.)

and then another file of the same name with `.cpp` extension.

Example: `player_hardware_SDL_mixer.h`

In the `.h` files you need to declare all the functions that `main.cpp` calls,
and then in their corresponding `.cpp` you need to write their definition.
Inside the implementations, you can also have other functions of your own if you want,
as long as they are not called by `main.cpp`. List of things to implement in each
header:

System:
<ul>
<li>void Init()</li>
<li>void FreeMusic()</li>
<li>void ClearScreen()</li>
<li>void UpdateScreen()</li>
<li>bool exitted</li>
<li>bool shouldLoop</li>
<li>Optional but recommended: a Quit() function to exit when asked</li>
</ul>

Player:
<ul>
<li>void PlaySource()</li>
<li>bool isPlaying()</li>
<li>void ProcessInput()</li>
</ul>

Source picker (can pick a file, can pick a web link, pretty much anything
as long as you can pass it to your player):
<ul>
<li>void selectSource()</li>
</ul>

And now you can make your own implementation knowing these!
(You can check `main.cpp` for a bit more info for each of these)