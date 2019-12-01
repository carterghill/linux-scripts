# Linux Scripts
Scripts I use to help automate basic tasks on my Linux machines (mostly my i3-gaps laptop). Most are c files which export their executables to ./bin. Run `make` in the terminal to compile them all, or `make [program name]` to compile a specific program (ex: `make brightness`). I recommend creating a dedicated folder for automated scripts like ~/.scripts and adding the executables to your bashrc. Open \~/.bashrc and add the line `export PATH=$PATH:~/.scripts/bin` to the bottom of the file, which will make these scripts useable in the terminal by typing the program name alone.

## brightness

Usage: `brightness [number]`

Programs like xbacklight don't seem to run properly on my laptop (Linux on a Macbook Pro). But, changing the settings file that stores the brightness directly DOES change the brightness on the fly. This program uses `system("cat /sys/class/backlight/intel_backlight/brightness")` and gets the current brightness from stdout. Then, adds the given number (can be negative) to that value and uses the echo command to replace the old data with our new brightness.

This brightness file may not be in the same location on your system, if this is the case you will have to edit brightness.c to make the program work. If you're using a computer with an Nvidia GPU, you might find it by replacing intel_backlight with nvidia_backlight.

You may get a permission error when running brightness. If so, run 
```
sudo crontab -e -u root
```
And add the following line:
```
@reboot chmod 777 /sys/class/backlight/intel_backlight/brightness
```
##### i3-gaps Users
Those who use i3 gaps on a Macbook like me can bind the keyboard brightness keys to this function by adding the following lines to ~/.config/i3/config:
```
bindsym XF86MonBrightnessUp exec --no-startup-id brightness 200
bindsym XF86MonBrightnessDown exec --no-startup-id brightness -200
```

##### To do
Since not all systems store their current brightness in the same folder, I would like to find a way to locate it automatically (maybe I can always find it within /sys/class/backlight/ somewhere).

Currently if you try to increase it past the maximum brightness, it simply won't take effect. I'd like to have it detect that the new brightness level is past the max and set it to that instead. The maximum would be easy to figure out for my system just through trial and error, but it probably isn't the same value on all systems so a way to figure it out through code would be better.

## Auto Background

Usage: `auto_background [path] [time]`

This program loops through all image files in a folder path and changes your background wallpaper after waiting the given amount of time.

Requires: `feh`

#### To do
Background is replaced instantly when the interval is over. I would like create a fading transition by using an increasing opacity on the original image with the new image drawn underneath.
