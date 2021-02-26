# now Playing
## What is it?
You know, you have just only one monitor, or have open important stuff on both of them. You're listening to a new album by {your favourite artist}, and some sick banger is playing. You want to know the song name (Darude - Sandstorm), but you're in important LOL match, or mining in Minecraft or whatever. There is no time to alt-tab and check what is playing. So, you can buy an Arduino of your choice, simple and cheap 1602 LCD display with I2C adapter, and create little device that tells you, what song is playing. Simple as that.

## How does it work?
This code can use basically any audio player, that can output metadata to files. I'm using [foobar2000](), since it's the best. And it supports plugins. One such plugin is [Now Playing Simple](https://skipyrich.com/w/index.php/Foobar2000:Now_Playing_Simple), which outputs whatever metadata you specify to a text file. It does this every time a song is changed.

My code then watches that file if it has been changed, then parses and send via serial to Arduino. It then displays the text. Simple as that.

# Setup
I'm asuming you will be using foobar2000 with afromentioned Now Playing Simple (NPS) plugin. Follow this carefuly, you'll in 99% find whatever caused your problem in here. Probably you just missed something. 

## Installing and configuring NPS
If you don't have NPS installed, follow [this guide](https://wiki.hydrogenaud.io/index.php?title=Foobar2000:How_to_install_a_component) on how to install foobar plugins. You can obviously download the plugin from [here.](https://skipyrich.com/w/index.php/Foobar2000:Now_Playing_Simple)

When you have your plugin installed and working, head back to _preferences_ again, and in the left menu on bottom, there should be "Now Playing Simple" tab. Open it. **Check** save to file, and enter a file's path. **COPY THE PATH**. Next, in the _formatting string_, you can enter anything you want. You just have to have **first line as artist, second line as title**. Otherwise, you'll get random results. You can enter this config, which I use, works, and also provides some extra stuff.
```
$if($not(%ispaused%),$if(%isplaying%,%artist%$crlf()%title%$crlf()%album%',' $padcut(%date%,4),)))
```
You can specify _On exit_ message, however this won't be displayed on LCD.

## Preparing the Arduino and LCD
If you bought your LCD without the I2C adapter, you need to install it. Just check the orientation of the module (the PCB should be under the screen), solder it on and you're done. 

Next, you'll need to install the [PCF8574 LCD library](https://github.com/mathertel/LiquidCrystal_PCF8574). You can find it in default _library manager_, or download and paste into _libraries_ folder manually. Here's a [how-to guide](https://www.arduino.cc/en/guide/libraries).

Wire to your Arduino's I2C pins and power. Next, upload the [test sketch](./lcd_test_sketch.ino). On the back of the module, there should be (probably blue) trimmer. You need to adjust that, until you can clearly see every character on the screen. This sets the contrast. **If set too low, you won't see anything. If set too high, you'll see white squares.** If you can't see anything, it's probably because of this. Take your time with this.

When you're done setting the contrast, you can upload the [main sketch](./foobar_nowPlaying.ino).

## Running the python script
Obviously, you'll need [python](https://www.python.org/downloads/) installed. Any latest version (tested on 3.8 and 3.9) of python3 should work. Next up, open command prompt in the folder with this program, and write 
```sh
py -m pip install watchdog
```
This will only external library needed. Since there is only one, we don't need _requirements.txt_. 

If everything is fine, go ahead and edit [settings file](./example-settings.py). **DO NOT FORGET TO RENAME IT**. 

Now you can run the program with 
```sh
python main.py
```
If you have multiple versions of python (i.e. _python2_ and _python3_) installed, you might need to specify which version to use:
```sh
python3 main.py
```

Everything should work now. Try opening foobar, and in about a second, info should appear on the LCD.

# Problems, ideas, etc
If there is a problem, don't be afraid to open an issue. Also new ideas can be presented there. Everything in the guide should be self-explanatory, code is commented. It is really simple, and there is not much you can screw up. If you do, re-read the guide. If you have new addition to code, create a pull request. 