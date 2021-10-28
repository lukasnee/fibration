<!-- ![Hopf Fibration](Misc/hopf-fibration.png) -->


                                       _____ __            __  _
                                      / __(_) /  _______ _/ /_(_)__  ___
                                     / _// / _ \/ __/ _ `/ __/ / _ \/ _ \
                                    /_/ /_/_.__/_/  \_,_/\__/_/\___/_//_/
                                            FOSS Modular Synth Framework

Open source modular synthesizer embedded software framework [WIP]

# Feature
- written in C++
- based on STM32 (F303xx atm) 
- based on FreeRTOS and interfaced over [ccp_freertos](https://github.com/michaelbecker/freertos-addons) wrapper
- DMA/I2S driven audio streamer for easy external ADC/DAC interfacing
- hardware FPU enabled CMSIS DSP support
- thread-safe and DSP oriented peripheral hardware resource manager (In progress)
- Common synth component high-level implementations (pot, slider, button, led, rotary encoder... TBD)
- lightweight command shell and logger over UART (nice for experimenting and debugging)
- *Tracealyzer* support (TBD)

# Framework

        +------------+     +-----------+     +------------+     +-------------+     +-----------+   
        | Modular    |     | Module #1 |     | Module # 2 |     | Module #... |     | Module #N |   
        | Synth      |     +----+------+     +-----+------+     +------+------+     +-----+-----+   
        | apps       |          |                  |                   |                  |         
        | (hardware) |          +------------------+---------+---------+------------------+         
        |            |                                       |                                      
        +------------+  +------------+-----------+-----------+------------+------------+-----------+
        | component  |  |  Audio In  | Audio Out |   CV In   |   CV Out   |  Gate In   | Gate Out  |
        | drivers    |  +------------+-----------+-----------+------------+------------+-----------+
        |            |  |  Clock In  |Clock Out  |  Button   |    Knob    |   Slider   |    LED    |
        |            |  +------------+-----------+-----------+------------+------------+-----------+
        |            |  |  Display   |  microSD  |    Mic    |    and     |    more    |    ...    |
        +------------+  +------------+----+------+-----------+------------+------+-----+-----------+
        | services,  |  |   Shell/Logger  |   AudioStream    |    CMSIS DSP      |     Variable    |
        | streams    |  |                 |                  |    Q-aritmetic    |                 |
        +------------+  +-----------------+------------------+-------------------+-----------------+
        | core,      |  |         Resources Manager          |     third-party libs (FAT32, etc.)  |
        | peripheral |  +------------------------------------+-------------------------------------+
        | hardware,  |  |               FreeRTOS / ccp_freertos / SYSTEMVIEW(opt.)                 |
        | interface  |  +-----------------+------------------+-------------------+-----------------+
        |            |  |    DMA / UART   |    DMA / I2S     |       HW FPU      |    ADC / GPIO   |
        +------------+  +-----------------+------------------+-------------------+-----------------+
        |            |  |                                STM32 HAL                                 |
        |  BSP/MSP   |  +--------------------------------------------------------------------------+
        |  hardware  |  |                               STM32F303XX                                |
        +------------+  +--------------------------------------------------------------------------+

# Modules

> Particle physics themed modular system ...heh

        +=========+=================================+    +=========================================+
        | Name    | Feature Set                     |    |           The Standard Model            |
        +=========+=================================+    +=========================================+
        | Higgs   |                ?                |    |           | Scalar  | Higgs Boson       |
        +---------+---------------------------------+    |           +---------+-------------------+
        | Photon  |                ?                |    |           |         | Photon            |
        +---------+---------------------------------+    |           |         +-------------------+
        | Gluon   |                ?                |    |  Bosons   |         | Gluon             |
        +---------+---------------------------------+    |           |  Gauge  +-------------------+
        | Wiggs   |                ?                |    |           |         | W Boson           |
        +---------+---------------------------------+    |           |         +-------------------+
        | Ziggs   |                ?                |    |           |         | Z Boson           |
        +---------+---------------------------------+    +-----------+---------+-------------------+
        | Quarp   |                ?                |    |           |         | Up                |
        +---------+---------------------------------+    |           |         +-------------------+
        | Charm   |                ?                |    |           |         | Charm             |
        +---------+---------------------------------+    |           | Quarks  +-------------------+
        | Fermit  |                ?                |    |           |         | Top               |
        +---------+---------------------------------+    |           |         +-------------------+
        | Dark    |                ?                |    |           |         | Down              |
        +---------+---------------------------------+    |           |         +-------------------+
        | Strange |                ?                |    |           |         | Strange           |
        +---------+---------------------------------+    |           +---------+-------------------+
        | Leptron |                ?                |    | Fermions  |         | Lepton            |
        +---------+---------------------------------+    |           |         +-------------------+
        | Muon    |                ?                |    |           |         | Muon              |
        +---------+---------------------------------+    |           |         +-------------------+
        | Tau     |                ?                |    |           |         | Tau               |
        +---------+---------------------------------+    |           | Leptons +-------------------+
        | Leptro  |                ?                |    |           |         | Tau Neutrino      |
        +---------+---------------------------------+    |           |         +-------------------+
        | Mutro   |                ?                |    |           |         | Muon Neutrino     |
        +---------+---------------------------------+    |           |         +-------------------+
        | Etro    |                ?                |    |           |         | Electron Neutrino |
        +---------+---------------------------------+    +-----------+---------+-------------------+

# Status
Experimentation phase, no releases yet.

# Licenses
- GNU GPL-3.0 for software
- CERN OHL-2.0 for hardware

# Development Enviroment

It started and mostly continued on Raspberry Pi4 (Raspbian OS (debian)) remotely using VSCode "Remote - WSL" extension (ms-vscode-remote.remote-wsl) from Windows PC. Yes, that is a pretty weird setup.

I managed to setup building/flashing/debugging experience on VSCode on Windows using Ubuntu WSL (Ubuntu enviroment on Windows, just google how to install WSL). The setup is almost identical for both Linux and WSL cases.

1. Install apt package prerequisites:

For Linux:
```shell
sudo apt install make cmake stlink-tools
```

For WSL:
```shell
sudo apt install make cmake
```
and follow `stlink-tools on WSL` workaround in a clause below.

2. Install GNU Arm Embedded Toolchain (`arm-none-eabi-*`) - see clause below.

3. Install `VSCode Extensions`:

- ms-vscode.cpptools
- twxs.cmake
- marus25.cortex-debug

## GNU Arm Embedded Toolchain
It is tempting to simply install GNU Arm Embedded Toolchain with oneliner like this:
```shell
sudo apt install gcc-arm-none-eabi
```

But this distribution always caused me problems - outdated and missing `arm-none-eabi-gdb`. Therefore I recommend installing it manually. Here's how [based on a very nice forum answer by Aleksander Khoroshko](https://askubuntu.com/questions/1243252/how-to-install-arm-none-eabi-gdb-on-ubuntu-20-04-lts-focal-fossa):

1. Make sure you don't have the GNU Arm Embedded Toolchain apt package on you system:
```shell
sudo apt remove gcc-arm-none-eabi
```

2. Download the latest version (Linux x86_64 Tarball) [from official website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads). Unpack it into any directory, for example:
```shell
sudo tar xjf gcc-arm-none-eabi-<VERSION>.bz2 -C /usr/share/
```

3. Create symbolic links so that binaries are accessible system-wide:
```shell
sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc 
sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb
sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy
sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-nm /usr/bin/arm-none-eabi-nm
# ... link other arm-none-eabi-* if you intend to use
```
4. Install and link dependencies (gdb uses ncurses):
```shell
sudo apt install libncurses-dev
sudo ln -s -f /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
sudo ln -s -f /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo
```

5. Check if tools are working:
```shell
arm-none-eabi-gcc --version
arm-none-eabi-g++ --version
arm-none-eabi-gdb --version
arm-none-eabi-size --version
arm-none-eabi-objcopy --version
arm-none-eabi-nm --version
```


## `stlink-tools` on WSL

The `stlink-tools` includes `st-flash` for flashing, `st-util` for launching GDB server for debugging.

As far as I know Ubuntu WSL does not support USB so `stlink-tools` apt package based on `libusb` won't work. You can try installing `stlink-tools` windows version binaries (based on Windows USB drivers) and make them accesible on WSL enviroment:

1. Make sure you don't have `stlink-tools` apt package installed on your WSL system: 
```shell
sudo apt remove stlink-tools
```
2. Download [st-link toolset binaries for windows](https://github.com/stlink-org/stlink/releases) manually
(`stlink-<VERSION>-x86_64-w64-*.zip`).
3. Unzip `st-*.exe` binaries into any directory, for example `C:/User/<user>/bin`.
4. Add directory path into Windows `Path` global variable (just google how).
5. Rename `st-*.exe` executable files by removing extension `.exe` suffix (`st-info`, st-flash`, etc.) to make it "Linux style".
6. Now you should be able to use `st-*` executables from within Windows WSL. `Utils\flash.sh` and other bash scripts should work now. Test by connecting ST-LINK over USB and:
```shell
st-info --probe
```
