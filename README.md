# TC5Ever

Simple mod to enable 2 Player CO-OP link play with Time Crisis 5. It works by simply intercepting TCP `connect(2)` calls and replacing the destination address with whatever you choose as player 2. 

I have tested multiplayer with:
1. Two instances of the game on a single computer
2. Over LAN
3. Over the internet (port forwarding required)

## Installation

Unzip `dinput8.dll` to the same directory where `TimeCrisisGame-Win64-Shipping.exe` exists. With a typical installation that would resolve to `<Your Folder Name>/TC5/Binaries/Win64`

As of right now, it is technically only needed for player to have this dll installed, but I will likely add additional fixes for the game into the dll that will benefit both players.

---

## Player 1 (Server)

To have player 1 host, make sure you have port `3742` open on the host pc, and start the game with `-netcoophost=1 -playside=1`.

### Launch script
```batch
:: Start1.bat
@start TimeCrisisGame-Win64-Shipping.exe -NOINI -Language=JPN -netcoophost=1 -playside=1
```

---

## Player 2 (Client)

To have player 2 connect to the host, launch the game with `-netcoophost=1 -playside=2 -address=<player 1 ip>`.

By default, this mod will try to connect to player 1 via `localhost` or `127.0.0.1`, so launching two copies of the game as player 1 and player 2 will work.

You should see a custom logging window appear if you have installed this mod correctly.

### Launch script
```batch
:: Start2.bat
@start TimeCrisisGame-Win64-Shipping.exe -NOINI -Language=JPN -netcoophost=2 -playside=2
```

## Future

Once I receive my Sinden Light Gun, I plan on adding features to make it feel as close to the original arcade experience as possible. Most likely this will mean getting the haptic recoil to operate correctly in all scenarios, as well adding good support for 2 player co-op using the same PC with two light guns attached.
