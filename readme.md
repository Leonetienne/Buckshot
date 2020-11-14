# Buckshot
The screenshot tool

Buckshot is an easy, open source tool for Windows to take screenshots of specific applications with a single keypress.  

Just press F2 and your currently focussed application will get screenshotted!

Fully configureable!  
I personally recommend to put a link to Buckshot.exe in your startup directory.

NOTE!! If your executeable is named something other than `Buckshot.exe`, it won't detect multiple instances properly.  
This is usually not a problem, but if Buckshot is open twice, it will take TWO screenshots!


## The config file
NOTE!! If you do not have a config file, run Buckshot once and it will create one for you.  
Do note that Buckshot runs in the background by default and you will have to either kill it via taskmgr or start another instance and click "kill old instance".  

Example config file with explanation
```
{
  # Show the debug console?
  "showDebugConsole": false,

  # All these VK-Ids have to be pressed at once.
  # Default is just F2
  "keybind": [
    0x71
  ],

  # Put all screenshots there
  "defaultSavePath": "C:\\Users\\Leon\\Pictures\\Buckshot",

  # Name the screenshots like this
  "defaultNamingScheme": "$(winName) - $(time).png",

  # Use whitelist or blacklist for application names?
  "useWhiteListInstead": false,

  # String-Arrays for window names
  "appWhitelist": [],
  "appBlacklist": [],

  # Custom naming schemes and save paths per apps
  "savePathsPerApp": [
	{
	  "app": "World of Warcraft",
	  "path": "F:\\Spiele\\World of Warcraft\\_retail_\\Screenshots"
	}
  ],
  "namingSchemePerApp": [
	{
	  "app": "World of Warcraft",
	  "scheme": "WoW $(unixtime).png"
	}
  ]
}
```

## Custom keybinds
The keybind is just a list of [Virtual Keycodes](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes).  
You can either use decimal (`123`) or hexadecimal (`0x7B`) values as key id's.

So, to use let's say ALT+DEL as a keybind, you would do
```
# `0x12` is the key code of ALT and `0x2E` is the key code of DEL
"keybind": [ 0x12, 0x2E ]
```

Common key codes are:
```
LSHIFT    : 0xA0
RSHIFT    : 0xA1
LCTRL     : 0xA2
RCTRL     : 0xA3
ALT       : 0x12
DEL       : 0x2E
F1-F24    : 112 - 136
0-9       : 48 - 57
0-9 numpad: 96 - 105
```

## Custom naming scheme
You can change how Buckshot names your screenshots. You can use these variables:
* `$(winName)`: The title of the window being screenshot
* `$(time)`: A human-readable timestamp
* `$(unixtime)`: A timestamp in unix time

You have to add the .png extension yourself! Example: `$(winName) - $(time).png`.  
Every illegal filename character will be removed.

## Custom save path
You can specify a custom path for your images to be saved at. The default is the directory `screenshots` in Buckshots working directory.  
NOTE!! You HAVE to use backslashes `\` AND because of the json file format you HAVE to escape `\`s as `\\`  
NOTE!! The path HAS to already exist!

## Custom per-app settings
You can override naming schemes and saving directories based on the applications name!  
Look at `savePathsPerApp` and `namingSchemePerApp`.  
NOTE!! The application name is literally just it's window's title!! NOT the executeables name!

# White and Blacklist
I don't have to explain to you what a white- and blacklist is, have i?  
Just know that the blacklist is used by default and to use the whitelist instead you have to set `useWhiteListInstead` to `true`.  
NOTE!! The application name is literally just it's window's title!! NOT the executeables name!  
NOTE!! This is **NOT** some sort of regex or glob thing! You **CANT** do stuff like `*Google Chrome*`!  Maybe i'll add that some day but today is not the day.

Example for a whitelist:
```json
"useWhiteListInstead": true,
"appWhitelist": [
    "World of Warcraft",
    "Call of Duty® WWII Multiplayer"
],
"appBlacklist": [],
```

## Debugging
Is something not working?  
Set `showDebugConsole` to `true`, restart Buckshot and see detailed debug output in the dev console.

## Legal notice PLEASE READ
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
