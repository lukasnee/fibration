# Maintenance

## Development Environment

### Linux (WSL)

I managed to set up a complete build+flash+debug Linux experience on Windows using [WSL](https://docs.microsoft.com/en-us/windows/wsl/install)2 Ubuntu distribution and [usbipd](https://github.com/dorssel/usbipd-win) for ST-LINK and serial console USB device access.
The following instructions should work for native Linux environment too - just skip steps marked as [WSL only].

1. [WSL only] Install WSL if using windows. I recommend installing WSL version 2.

1. Install common apt package prerequisites:

    ```shell
    sudo apt install git make cmake clang-format openocd stlink-tools
    ```

1. Install GNU Arm Embedded Toolchain (`arm-none-eabi-*`) - see clause below.

1. Install [Visual Studio Code](https://code.visualstudio.com/) and `ms-vscode-remote.remote-wsl` extension if using WSL.

1. [WSL only] Install `ms-vscode-remote.remote-wsl` extension if using WSL.

1. Navigate to where you want to install *fibration* MDK (`cd ~` if not sure) and do:

    ```shell
    git clone https://github.com/lukasnee/fibration.git
    cd fibration
    git submodule update --recurse --init
    git checkout -b my-branch
    code .
    ```

    > A VSCode windows should start loading. In WSL case, make sure VSCode starts in WSL mode (in the bottom left corner you should see a colored box with text: `[WSL: Ubuntu]`

1. Install following VSCode extensions:

    - `ms-vscode.cpptools`
    - `twxs.cmake`
    - `marus25.cortex-debug`
    - `xaver.clang-format`

    > Make sure you install these extensions on VSCode WSL service side.

1. Build project `higgs` by running VSCode tasks `build higgs [release]` or run manually using `./utils/build.sh -p higgs -t release`.

    > Initial build can fail so try building a couple times more.

1. Flash project `higgs` by running VSCode tasks `build and flash higgs [release]` or run manually using `./utils/flash.sh -p higgs -t release -b`.

1. That's it!

### GNU Arm Embedded Toolchain

It is tempting to simply install GNU Arm Embedded Toolchain with one-liner like this:

```shell
sudo apt install gcc-arm-none-eabi
```

But this distribution always caused me problems - outdated and missing `arm-none-eabi-gdb`. Therefore, I recommend
installing it manually. Here's how
[based on a very nice forum answer by Aleksander Khoroshko](https://askubuntu.com/questions/1243252/how-to-install-arm-none-eabi-gdb-on-ubuntu-20-04-lts-focal-fossa):

1. Make sure you don't have the GNU Arm Embedded Toolchain apt package on you system:

    ```shell
    sudo apt remove gcc-arm-none-eabi
    ```

2. Download the latest version (Linux x86_64 Tarball)
   [from official website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).
   Unpack it into any directory, for example:

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
    sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-objdump /usr/bin/arm-none-eabi-objdump
    sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-nm /usr/bin/arm-none-eabi-nm
    sudo ln -s /usr/share/gcc-arm-none-eabi-<VERSION>/bin/arm-none-eabi-ar /usr/bin/arm-none-eabi-ar
    ## ... link other arm-none-eabi-* if you intend to use
    ```

4. Install and link dependencies (gdb uses ncurses):

    ```shell
    sudo apt install libncurses-dev
    sudo ln -s -f /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
    sudo ln -s -f /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
    ```

5. That is it ! Check if tools are working using:

    ```shell
    arm-none-eabi-gcc --version
    arm-none-eabi-g++ --version
    arm-none-eabi-gdb --version
    arm-none-eabi-size --version
    arm-none-eabi-objcopy --version
    arm-none-eabi-objdump --version
    arm-none-eabi-nm --version
    arm-none-eabi-ar --version
    ```

### OpenOCD on WSL

Please follow these instructions: [WSL: Connect USB devices](https://learn.microsoft.com/en-us/windows/wsl/connect-usb).
