# Spelunky 2 Seed Finder

Search for specific Spelunky 2 seeds that satisfy certain restrictions. Look for seeds with many altars, shops with certain items, an accessible pet, a jetpack in the crust, ... Look below for a description of all the provided filters.

This project is built on top of [Overlunky](https://github.com/spelunky-fyi/overlunky), be sure to read all their instructions and documentation as well. 

![Spelunky 2 Seed Finder](https://user-images.githubusercontent.com/16322135/111480373-3909f300-8732-11eb-94db-c12431c525ec.gif)

## Disclaimer
Please read the [disclaimer of the Overlunky project](https://github.com/spelunky-fyi/overlunky#disclaimer) first. To reiterate, don't bother BlitWorks or Mossmouth with any bugs/breakage that may result through the use of this project. In addition, the author is not responsible for anything that might break by running the Seed Finder, especially considering the following warning. Use at your own risk!

## WARNING
When using the Seed Finder on the early levels (e.g. solely on 1-1) they will load very quickly, creating flashes on the screen. There seems to be 1 frame of complete white for each level that is loaded. In testing the Seed Finder, I had let it run for a couple thousand seeds, and when it finished, the portion of the screen where Spelunky 2 resided **kept flashing for about an hour afterwards**, even after shutting down the monitor and PC. 

To alleviate this a bit, an 'Anti flicker frames' configuration option was added, where you can adjust how many frames the Seed Finder must wait after loading a different seed. Be safe, run Spelunky 2 in windowed mode, and drag it completely off screen when searching for seeds. Don't complain if this broke your monitor or pc!

## Installation
Download the release, and unzip it. Copy the exe and dll file into your Spelunky 2 folder (otherwise it will complain it can't find MSVCP140.dll). Run the executable, then start Spelunky 2. The Seed Finder window should pop up a bit after Spelunky 2 starts. To actually use the Seed Finder, you must continue until you are inside a level.
Do not combine with any other mods at the same time, just run the Seed Finder to find some seeds, quit, and either play regular Spelunky 2 or use Overlunky, ...

## Documentation

![Spelunky 2 Seed Finder Main Window](https://user-images.githubusercontent.com/16322135/111478060-019a4700-8730-11eb-8eb9-0e9610c1cac4.png)

### Log file
Every time you press the "Find Seeds" button a log file called seed-finder.log will be appended to. This file can be found next to the Spelunky 2 executable. You'll find all sorts of information there regarding the search, as well as a list of all the seeds that matched and the total time it took. 

### Export / Import
At the top right of the Seed Finder window, you'll see an Import button, as well as an Export button (when you have correctly configured a filter). This is so you can easily store a set of filters and configuration to reuse later or to send somewhere. Just copy/paste the generated JSON information to export and import.

### Configuration

#### Amount of seeds to process
This determines how many seeds should be tested against your filters. Start with a low amount and see how it goes. You can get an idea of how long it will take to do a large number of seeds by looking at the seeds/sec statistic at the bottom of the log file. Make sure to read the WARNING above when you choose a large number.

#### Generate random seeds or start at seed
You can let the Seed Finder pick a random number out of the 4.3 billion possible seeds, or you can tell it a specific number to start at (uncheck the 'random'-checkbox first so you can type in the text field). If you provide your own starting seed, it will simply count upwards from that number.

#### Anti flicker frames
The amount of frames that should be waited before changing to a new seed. Read the WARNING above to understand why this could be necessary. Put at 0 at your own risk, and be sure to pop the Spelunky 2 window off screen while you search !

### Filter(s)
Here you will define what parameters a seed needs to satisfy in order to be retained. The drop down lists all filters available, just pick one and click the Add button. There's a little X button at the top right of each filter to remove it from the list.

You can add as many filters as you like, but make sure what you ask for actually exists in the Spelunky universe ! Searching for a seed with altars on each and every level might be fun, but it's highly unlikely that such a seed exists, so you'd be wasting your time.

Look below for a description of what all the filters do.

When you are satisfied with your filter selection, click the "Find Seeds"-button and Spelunky will start searching. When it's finished, a list of matching seeds will be displayed. You can click on these buttons to immediately bring you to the first level of that seed. It's recommended though to play the seeds without the Seed Finder being loaded.

#### Filter: Find altar
* Makes sure there's an altar on the level
* You can choose whether the altar is accessible to the player or not, whether it is in a closed off section or not
* Choose all the levels on which an altar must appear

#### Filter: Find Ankh skip
* Looks for a way to skip losing the Ankh in the Tide Pool
* Check for back layer doors under the lava: although I have seen this happen before, it may have been patched out, as I cannot find a seed anymore that has a back layer door below the lava (apart from the one-way door, obviously)
* Check for pagoda at the edge: makes it easier to rope and grab onto the side of the level wall, with some crafty usage of bombs

#### Filter: Find black market
* Gives you the choice on which level in the jungle the black market should appear on

#### Filter: Find caveman shop with specific contents
* Choose up to three items that must be in the caveman's shop
* Pick the levels on which the shop must appear (you'll probably want to pick only one level though, as the shop with the requested item(s) must appear on all picked levels!)

#### Filter: Find crate with specific contents
* Choose the item that must be in the crate, as well as the amount of such crates that must exist *within each level you select*
* Choose which layer the crate must appear on
* Pick the level(s) on which x amount of these crates must appear

#### Filter: Find dice shop
* Pick all the levels on which a dice shop must appear
* Note that you can't pick the first item that appears in the shop, this is a known limitation (see below)

#### Filter: Find entity
* Gives you the ability to look for a certain amount of a specific entity in a level, on either back or front layer or both
* You can use this if there's no specific filter available for this type of entity, e.g. you want 3 Lamassus on one particular level

#### Filter: Find ghost jar
* Makes sure there's a ghost jar on the level
* You can choose whether the ghost jar is accessible to the player or not, whether it is in a closed off section or not
* Choose all the levels on which a ghost jar must appear

#### Filter: Find item in crust
* Looks for the specified item buried in the crust, for which you'd need to use a bomb or the mattock to release it
* Choose the layer and on which level it must appear

#### Filter: Find level feelings
* Choose whether or not a specific level should have darkness, voice echo, a bee hive, restless dead, clanking metal or an air of opression

#### Filter: Find Moon challenge
* Finds seeds on which the Moon challenge is on the specified level (or any level in combination with the other restrictions)
* You can specify whether the durability of the mattock should greater or less than a number (it seems the durability ranges from 40 to 60 swings)
* You can choose whether the moon challenge must be accessible (it probably always is though, due to forced level generation in that way)

#### Filter: Find pet
* Choose whether the pet must be accessible without bombs (or not behind a locked back layer door) on the specified level(s)

#### Filter: Find pot with specific contents
* Looks for a throwable pot with a certain item in it, and the amount of which there must exist

#### Filter: Find Quillback obstruction
* Checks whether the push block at the bottom of the Quillback level exists or not, either on the Jungle or Volcana side, or both

#### Filter: Find regular shop with specific contents
* Choose up to four items that must be in a shopkeeper's shop
* Choose the back layer to find items in the black market
* Pick the levels on which the shop must appear (you'll probably want to pick only one level though, as the shop with the requested item(s) must appear on all picked levels!)

#### Filter: Find sisters
* Checks whether the door that accesses Parsley, Parsnip, and Parmesan is accessible without bombs
* Pick every level for which the accessibility must be checked

#### Filter: Find Sun challenge
* Check whether the sun challenge is accessible without bombs
* You can also find seeds on which the Sun challenge is on a particular level

#### Filter: Find Tun shop with specific contents
* Choose up to three items that must be in Tun's shop in the back layer
* Pick the levels on which the shop must appear (you'll probably want to pick only one level though, as the shop with the requested item(s) must appear on all picked levels!)

#### Filter: Find Udjat Eye
* Choose which level the Udjat Eye must appear on, or choose Any to combine it with the other filter restrictions
* You can pick whether the key and the door to the chest must be accessible without bombs or not
* You can also pick whether the key must reside above or below the Udjat door in the level

#### Filter: Find Van Horsing
* Choose whether the door to access Van Horsing must be accessible without the use of bombs or not
* You can also choose whether a door to a key must be accessible or not. In levels with multiple keys: if you choose must be accessible, then at least one key must be accessible. If you choose must not be accessible, then none of the keys must be accessible

#### Filter: Find vault
* Makes sure there's a shopkeeper vault on all the levels that are picked

#### Filter: Find Volcana drill
* Choose which level the drill should appear on, or pick Any to combine it with any of the other filter restrictions
* Choose the amount of lava blocks underneath the drill - not only the lava directly in the drill path is counted, but also each horizontally connected lava block (as that lava would also contribute to the lava dropped down)
* You can also filter out seeds that have or have no shop or altar under the drill path


## Known limitations
* Can't filter the first item in the dice shop, because it isn't immediately available on the first frame of the level. It pops out of the prize dispenser (you can see it bounce if it's in view when the level loads).
* Can't filter on the accessibility of Waddler in 7-1, because his presence is determined by having dropped off something with him earlier. Maybe Waddler's door is always accessible though.
* You can't set the shopkeeper aggro level or any other quest flag that influences level generation later on (occurrance of shopkeeper outposts, Sparrow's hideout, ...)
* The deeper a level you choose, the longer it takes for each seed to process. That's because every preceeding level needs to be generated, you can't just skip to e.g. 4-2, because the level generation will be different compared to if you followed the path from 1-1, 1-2, ... to 4-2. You can experiment with this yourself by using Overlunky: load a specific seed, and play the first two levels, look at what 1-3 looks like. Then restart the seed, and create a warp door to 1-3, and you'll notice it looks completely different. So, unfortunately it seems that individual levels depend on the preceeding levels. When you request a filter that applies to level 7-4, all the preceding levels have to be generated, taking up a lot of time.

## Troubleshooting
* On launch, it shows an error dialog because it's missing a dll file → copy the Spelunky2SeedFinder.exe and .dll files into your Spelunky 2 folder (next to Spel2.exe) and launch it from there.

## Contact
* [@zappatic](https://twitter.com/zappatic) on Twitter
* In case of a bug, please create a Github issue and provide the exported JSON of your filter and describe the problem in detail
* If you have an idea for a filter, let me know
