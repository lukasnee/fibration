#!/usr/bin/env python3

import sys
import argparse
import threading
import tty
import termios
import glob

try:
    import serial
except ImportError:
    print("pyserial is required. Install with: pip install pyserial")
    sys.exit(1)


def find_serial_devices():
    """Find available serial devices"""
    devices = []
    for pattern in [
        "/dev/ttyUSB*",
        "/dev/ttyACM*",
        "/dev/ttyS*",
        "/dev/serial/by-id/*",
        "/dev/serial/by-path/*",
    ]:
        devices.extend(glob.glob(pattern))
    return sorted(devices)


def main():
    parser = argparse.ArgumentParser(
        description="Live serial communication using pyserial"
    )
    parser.add_argument("device", nargs="?", help="serial device path")
    parser.add_argument(
        "-b", "--baudrate", type=int, default=921600, help="baudrate (default: 921600)"
    )

    args = parser.parse_args()

    if args.device:
        device = args.device
    else:
        devices = find_serial_devices()
        if len(devices) == 0:
            print("No serial devices found")
            sys.exit(1)
        elif len(devices) == 1:
            device = devices[0]
        else:
            print("Detected multiple serial devices:")
            for i, dev in enumerate(devices):
                print(f"  {i+1}: {dev}")
            while True:
                try:
                    choice = input("Select device (1-{}): ".format(len(devices)))
                    if 0 >= int(choice) or int(choice) > len(devices):
                        raise ValueError
                    idx = int(choice) - 1
                    device = devices[idx]
                    break
                except (ValueError, KeyboardInterrupt):
                    print("\nExiting...")
                    sys.exit(0)
    old_settings = termios.tcgetattr(sys.stdin)
    stop_reading = threading.Event()

    try:
        ser = serial.Serial(device, args.baudrate, timeout=0.1)
        print(f"Device: {device}")
        print(f"Baudrate: {args.baudrate}")
        print("Ctrl+C to exit")

        def read_serial():
            while ser.is_open and not stop_reading.is_set():
                if ser.in_waiting > 0:
                    data = ser.read(ser.in_waiting).decode("utf-8", errors="ignore")
                    data = data.replace("\r\n", "\n").replace("\n", "\r\n")
                    sys.stdout.write(data)
                    sys.stdout.flush()

        read_thread = threading.Thread(target=read_serial)
        read_thread.start()
        tty.setraw(sys.stdin.fileno())

        while True:
            char = sys.stdin.read(1)
            if ord(char) == 3:  # Ctrl+C
                break
            ser.write(char.encode("utf-8"))

    except (serial.SerialException, KeyboardInterrupt):
        pass
    except Exception as e:
        print(f"\r\nUnexpected error: {e}")
    finally:
        stop_reading.set()
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
        if "ser" in locals() and ser.is_open:
            ser.close()
        if "read_thread" in locals():
            read_thread.join(timeout=1.0)


if __name__ == "__main__":
    main()
