Since you have mastered C fundamentals and built a GUI app, you are at a perfect crossroads. You can either go deeper 
into C or expand your project portfolio.Here are three strong directions you can take next:1. Build a Text Editor (The 
Classic C Project)Building a terminal-based text editor from scratch is a rite of passage for C programmers. It will 
force you to interact directly with the terminal window and manage complex data structures in memory.What you will 
learn: Raw terminal modes, handling keyboard inputs instantly, and managing a buffer of text using dynamic arrays or 
linked lists.Great Resource: Follow the famous, free guide "Build Your Own Text Editor" (kilobytes.by/kilo), which walks 
you through writing a terminal editor in pure C with zero external libraries.2. Move to K.N. King’s Final Chapters 
(Advanced C)Chapters 21 to 27 of King’s book transition away from syntax and focus heavily on the standard C library, 
building large projects, and low-level bit manipulation.Chapter 22 (Files/Streams): Update your GTK4 calculator to save 
a rolling "history log" to a local text file, or read configuration settings from a file on startup.Chapter 20 & 23 
(Bitwise Operations): Write a program that compresses files using basic encoding, or build a tool that parses raw binary 
files (like reading the header data of a BMP image file).3. Build a Native System Monitor in GTK4Instead of a 
calculator, build a graphical dashboard that displays your computer's live stats (CPU usage, RAM usage, and running 
processes).What you will learn: How to read live system files (like /proc/stat on Linux), and how to use GTK4 timers 
(g_timeout_add) to refresh your UI elements automatically every second without blocking the application.Which of these 
paths sounds the most exciting to you: building a terminal text editor, diving into file/binary manipulation in King's 
book, or making a live system monitor in GTK4?3 sitesC Programming for Beginners: Step by Step Learning RoadmapFollow a 
clear step-by-step roadmap to learn C programming for beginners. Master basics, syntax, logic building, memory concepts,
...NareshITBuild awesome GUI’s with C++. Welcome back! As most of you know, C++…Oct 9, 2021 — Once you have the project 
downloaded on your machine you can start developing out your GUI! Below is an example of this project (f...MediumI Want 
to Learn Programming but I Don’t Know Where to StartSep 14, 2020 — It is another solid language to start your 
programming journey with. C can be called a “middle level” language. This means that un...Level Up CodingYou said: i do 
like al three but probably the third...can you point me in the right direction without giving anything anywayBuilding a 
system monitor in GTK4 is an excellent choice. It bridges low-level operating system data directly with a modern 
graphical interface.To build this without giving away any code or solutions, you can break the project down into three 
independent exploration phases.Phase 1: Locating the System DataBefore writing any UI code, you need to find where your 
operating system keeps its live performance data.If you are on Linux, the OS treats everything as a file. Open your 
terminal and look into the /proc directory. You will want to research and look inside these specific files using the 
cat command:/proc/stat (This holds CPU data)/proc/meminfo (This holds memory/RAM data)Your first goal: Write a 
standalone, tiny C program (no GUI) that uses standard C file operations (fopen, fgets, fscanf) to read these files, 
parse out the numbers, and print them to the terminal. Hint: CPU usage is not a single number; you will need to read 
the numbers and do a quick math formula based on total time vs. idle time.Phase 2: Mastering the GTK4 ClockA system 
monitor is useless if it only captures data once when the app opens. You need the UI to update automatically every 
second (or half-second).In GTK4, you cannot use standard C functions like sleep() because they will freeze the entire 
user interface. Instead, you must let the GTK main loop handle time.Research the GLib function g_timeout_add() or 
g_timeout_add_seconds().Learn how to pass a callback function to it. This function will act as your "heartbeat," running
 your parsing logic and updating your widgets at regular intervals.Phase 3: Choosing Your WidgetsOnce you can fetch the 
 data and trigger a recurring timer, you need to decide how to display it visually. Look into the GTK4 documentation for 
 these specific widgets to see which fits your design:GtkLabel: The simplest way to display text stats (e.g., "RAM: 4.2 
 GB / 16 GB").GtkProgressBar: Excellent for a visual filling bar showing the percentage of CPU or RAM currently in 
 use.GtkLevelBar: Similar to a progress bar, but often used for discrete capacity levels.As you get started, which piece 
 do you want to tackle first: exploring the /proc files to understand the math, or researching g_timeout_add to see how 
 GTK handles loops?1 siteThe Ultimate Guide to Ubuntu Performance MonitoringApr 4, 2025 — /proc/stat - CPU and System 
 Statistics Shows CPU time allocation (user, nice, system, idle, iowait) and other system counters.Last9You said: it 
 would be cool to add to this a way to connect to a arduino and get a signal from a sensor to display on the appThat is 
 an awesome idea. Adding an Arduino sensor feed turns your software project into a hardware project, making it feel like 
 a real industrial dashboard.To make this happen, your computer and the Arduino will talk using Serial Communication 
 over a USB cable.Here is how you can direct your research to make this work without spoiling the code:1. The Arduino 
 Side: Keeping it SimpleOn the Arduino, your only job is to read the sensor and dump that data straight out to the USB 
 port.Look up the Serial.begin() and Serial.println() functions in the Arduino documentation.Your goal: Program the 
 Arduino so that when you open the Arduino IDE's "Serial Monitor," you just see a steady stream of numbers scrolling 
 down the screen (like a temperature or distance reading) every second.2. The C Side: Reading the Serial Port as a 
 FileIn Linux and Unix-like operating systems, hardware devices are treated exactly like files. Your USB connection to 
 the Arduino will show up as a file in the /dev/ directory.Plug in your Arduino and list your devices in the terminal 
 (usually it will appear as something like /dev/ttyUSB0 or /dev/ttyACM0).Your goal: Write a small standalone C program 
 that uses standard C file operations (fopen) to open that device path and read the incoming characters using fgets or 
 fscanf.3. The GTK4 Integration: The Trap to AvoidOnce your C program can read the Arduino data, you will want to bring 
 it into GTK4. However, there is a hidden trap here:Reading from a serial port can be slow. If your C code pauses to 
 wait for the Arduino to send data, it will block the GTK main loop, causing your calculator or system monitor window to 
 freeze and stop responding.To solve this safely, look up g_io_add_watch() or GIOChannel in the GLib/GTK4 documentation. 
 These tools tell GTK: "Let me know automatically when the Arduino file actually has new data ready to read, so I don't 
 waste time waiting for it."Do you already have a specific sensor in mind that you want to read (like temperature, 
 light, or distance), or do you want to start by just sending test numbers from the Arduino to verify the connection?6 
 sitesRaspberry Pi Arduino Serial Communication - Everything You Need To KnowNov 11, 2019 — The Raspberry Pi and Arduino 
 will be connected with a USB cable and use Serial communication to talk to each other.The Robotics Back-EndHow to make 
 a hardware monitor ??? : r/arduinoFeb 25, 2024 — A typical system would be: the arduino board would be connected to USB 
 and you'd have a program that reads the data you want and w...RedditUsing a Hall Sensor with Arduino | GPIO.CC Learning
 Mar 2, 2020 — 1. Read the Input from the Sensor Just like in the tutorial Arduino for Absolute Beginners, we use Pin 2 
 to detect the digital sig...GitHub Pages documentationShow allYou said: can i do it with an atmega and baremetal cYes, 
 you absolutely can! In fact, writing bare-metal C on an ATmega chip fits perfectly with your background from K. N. 
 King's book. You will bypass the entire Arduino framework and talk directly to the microcontroller's hardware 
 registers.To build the firmware this way, you will swap out the Arduino IDE for tools like avr-gcc (the C compiler for 
 ATmega) and avrdude (the tool that flashes your code onto the chip).Here is the exact direction to take your research 
 for the bare-metal C side:1. The Core Concept: Memory-Mapped I/OIn chapter 20 of King's book, you learned about bitwise 
 operators (&, |, ~, <<). You will use these constantly in bare-metal C. Microcontrollers control their pins and 
 hardware modules by reading and writing to special, fixed memory addresses called registers.2. Research the UART / 
 USART PeripheralThe hardware inside the ATmega that handles serial communication over USB is called the USART 
 (Universal Synchronous and Asynchronous Receiver and Transmitter). You will need to download the datasheet for your 
 specific chip (like the ATmega328P found on the Arduino Uno) and read the USART chapter.You will need to learn how
 to:Set the Baud Rate: You must configure the clock speed of the serial connection so it matches your PC. Look up the
UBRR0 (USART Baud Rate Register) in the datasheet.Enable the Transmitter: You have to turn on the transmitter
hardware. Look up the UCSR0B register and its TXEN0 bit.Send a Byte: To send a character, you check if the
transmission buffer is empty, and then dump your character byte into a specific register. Look up UCSR0A (for the
status flag) and UDR0 (the I/O Data Register).3. Reading the Sensor NativelyInstead of using Arduino's analogRead(), you 
will interact directly with the ATmega's ADC (Analog-to-Digital Converter) peripheral.You will look up registers like 
ADMUX to select which hardware pin the sensor is plugged into.You will use ADCSRA to start the conversion process and 
wait for the hardware to finish turning the analog voltage into a digital number.How it ties back to GTK4From your PC's 
perspective, nothing changes! Whether you use the Arduino framework or pure bare-metal C on the ATmega, the chip still 
sends raw bits over the USB cable. Your GTK4 app will read /dev/ttyUSB0 exactly the same way.Do you already have a 
hardware programmer (like an AVR ISP), or do you plan to use an existing Arduino board as the target hardware to flash 
your bare-metal C onto?More5 sitesInterrupts and Memory Mapped I/O :: Bare Metal Programming Series 2Mar 21, 2023 — This 
is episode 2 of the bare metal programming series! In this video, we're taking a deep dive into the memory-mapped I/O 
mechani...YouTubePython I2C Programming on the Raspberry PiMay 9, 2020 — Control of these I/O pins is done by reading 
and writing to registers within the chip. A register is a section of memory, in this ...AB Electronics UKAvr Atmega Gps 
Module Interfacing With Atmega1632 | Avr AtmegaIn this interfacing, AVR based ATmega microcontroller will read data 
serially from GPS receiver using USART communication with a 9...ElectronicWingsShow allYou said: i dont but i should get 
one, as well as some atmega chips but i do have arduinos i could useYou do not actually need to buy a separate hardware 
programmer yet! You can use your existing Arduino boards right now to run bare-metal C.The Arduino board is essentially 
just an ATmega328P chip sitting on a breakout board with a built-in USB-to-serial converter. When you write pure, 
bare-metal C, you can flash it directly over the USB cable using the standard bootloader already on the board.Here is 
the exact roadmap to set up a bare-metal C workflow using your Arduino hardware.1. The Toolchain to InstallInstead of 
the Arduino IDE, you will use standard terminal command-line tools to compile your C code. You will need to 
install:avr-gcc: The standard GNU C compiler, configured specifically to target AVR microcontrollers.avr-libc: The 
standard C library for AVR, which provides header files like <avr/io.h> so you can use register names instead of raw 
memory addresses.avrdude: The utility software that actually uploads the compiled binary file to the ATmega chip.2. How 
the Compilation Pipeline WorksYour workflow will look exactly like compiling code on your PC, but with an extra step to 
convert the file format:Compile: Run avr-gcc with the -mmcu=atmega328p flag to compile your .c file into an .elf object 
file.Convert: Use a tool called avr-objcopy to strip out debugging data and convert the .elf file into a lean, raw .hex 
file.Flash: Run avrdude. You will tell it to communicate with your board via its serial port (e.g., /dev/ttyACM0) using 
the arduino protocol flag. It will push your .hex file directly onto the chip.3. Your Very First Bare-Metal TestBefore 
tackling the complex serial registers or your GTK4 app, start with the microcontroller equivalent of "Hello, World!": 
blinking the onboard LED.On an Arduino Uno, the built-in LED is connected to digital pin 13, which maps directly to Port 
B, Bit 5 on the ATmega328P chip. Your first bare-metal program will look something like this:Include <avr/io.h> to get 
access to the register names.Use the DDRB (Data Direction Register B) to configure Bit 5 as an output pin.Use a while(1) 
loop to infinitely toggle the bit in the PORTB register, using the exact bitwise operators (<< and ^) you learned in 
Chapter 20 of King's book.Include <util/delay.h> to pause between toggles so you can see the blinking.Once you 
successfully flash a blinking LED using nothing but a terminal and command-line tools, you will have a fully 
functioning bare-metal toolchain ready for your system monitor project.Are you using Linux, macOS, or Windows on your 
development PC? I can give you the specific package manager command to install avr-gcc and avrdude for your system.17 
sitesDoes Arduino Need to be Connected to PC? - General GuidanceJan 10, 2019 — Yes, on the Arduino board Tx0 Tx1/Rx0 Rx1 
goes to the onboard serial to USB converter which goes to the host PC via USB. It can be...Arduino ForumHelp create a 
how-to guide: Upload program written for Arduino to ATmega328PJan 14, 2012 — They ( people ) want to code in the Arduino 
language and then port that code to a basic ATmega328 ( ATmega328P ) or equivalent chi...Arduino ForumdipDuino Is an 
Arduino-Compatible Board That's as Slim as a DIP ChipMay 27, 2020 — AtomSoftTech says the board functions like an 
Arduino Pro Mini. But we think it seems much more like a Nano since it contains an-o...Hackster.ioShow allYou said: wsl 
ubuntu on windowsPerfect. WSL Ubuntu makes installing the AVR toolchain incredibly easy because the standard Ubuntu 
repositories have everything you need built right in.One major thing to know when using WSL: by default, WSL cannot see 
USB devices plugged into your Windows machine. You have to explicitly "attach" your Arduino to Ubuntu using a free 
Microsoft tool called usbipd-win.Here is the exact setup path for your WSL Ubuntu terminal.Step 1: Install the AVR 
Compiler ToolsRun this standard command in your Ubuntu terminal to grab the entire compiler pipeline:bashsudo apt update 
&& sudo apt install gcc-avr avr-libc avrdude -y

Use code with caution.Step 2: Bridge your Arduino into WSLTo let your Ubuntu compiler talk to the physical Arduino USB 
port, follow these quick steps on your host machine:On Windows (PowerShell as Admin): Install the USB bridging tool by 
running:powershellwinget install winget-cli # (if winget isn't active)

winget install dorssel.usbipd-win
Use code with caution.Restart your PC to finish the tool installation.Plug in your Arduino via USB.On Windows 
(PowerShell as Admin): Find your Arduino's Bus ID and bind it:powershellusbipd list   # Look for your Arduino/CH340/FTDI 
device and note the BUS ID (e.g., 2-4)
usbipd bind --busid <BUS-ID>
Use code with caution.Attach it to Ubuntu: Every time you plug the board in, run this in PowerShell to send it over to 
WSL:powershellusbipd attach --wsl --busid <BUS-ID>
Use code with caution.Once attached, go to your WSL Ubuntu terminal and type ls /dev/tty*. You should see either 
/dev/ttyUSB0 or /dev/ttyACM0 pop up. That is your physical Arduino!Step 3: Write a Quick MakefileBecause bare-metal 
compilation requires a few flags, C developers typically automate it. Once you are ready to compile, you can create a 
simple text file named Makefile in your project folder that looks like this:makefiledefault:
	# 1. Compile the C code for the ATmega328P
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c main.c -o main.o
	avr-gcc -o main.elf main.o
	
	# 2. Convert it to a raw hex file
	avr-objcopy -O ihex -R .eeprom main.elf main.hex
	
	# 3. Flash it over the bootloader (change ttyACM0 if yours is ttyUSB0)
	avrdude -F -V -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex
Use code with caution.Whenever you want to build and flash your code, you just type the single word make in your 
terminal.Would you like instructions on how to write the bit manipulation code to turn on that digital pin 13 LED, or 
would you prefer to get the usbipd tool configured on your system first?10 sitesRunning Kali Linux on WSL 2? Here’s How 
to Connect External USB Wi-Fi or PenDrives (usbipd Guide)Dec 9, 2025 — However, WSL 2 users face one major limitation: 
Lack of direct hardware access. This means if you plug a USB Wi-Fi Adapter or a Pe...MediumWSL gives you no support for 
USB devices, which is a massive pain for embedded d...May 19, 2025 — WSL gives you no support for USB devices, which is 
a massive pain for embedded development when IT forces you to use Windows. Also...Hacker NewsHow to connect USB devices 
in Windows Subsystem for Linux under Windows 11Apr 3, 2023 — Thanks to Microsoft's contributions to the open-source 
usbipd-win project, you can now enjoy USB pass-through access in WSL. It me...XDAShow allYou said: greaet i like this 
project can you sum it all up in a neat form that i can cpyHere is a complete, structured blueprint for your Bare-Metal 
C & GTK4 System Monitor project. You can copy this directly into your notes or a markdown file.Project Blueprint: 
Bare-Metal C & GTK4 Hardware Monitor🚀 Project OverviewA desktop system monitor application built in pure C using GTK4. 
It captures local computer metrics (CPU & RAM) while simultaneously reading live telemetry data from a physical sensor 
connected to an ATmega328P (Arduino Uno) microcontroller programmed in bare-metal C.🛠️ Phase 1: The WSL Ubuntu 
Toolchain Setup1. Install AVR Tools in Ubuntubashsudo apt update && sudo apt install gcc-avr avr-libc avrdude make -y
Use code with caution.2. Bridge Windows USB to WSLRun this in Windows PowerShell (Admin) every time you plug in your 
hardware:powershell# 1. Find the Bus ID of your Arduino
usbipd list

# 2. Bind the device (only needed the first time)
usbipd bind --busid <BUS-ID>

# 3. Attach it to your running WSL instance
usbipd attach --wsl --busid <BUS-ID>
Use code with caution.Verify in WSL using: ls /dev/ttyACM* or ls /dev/ttyUSB*🔬 Phase 2: The Firmware (Bare-Metal C)1. 
Core Logic RoadmapHeader Files: Use <avr/io.h> for register mapping and <util/delay.h> for timing.Bitwise I/O 
Configuration: Configure hardware registers (DDRB, PORTB) using bitwise operations (<<, |, &, ^) to initialize 
pins.USART (Serial) Configuration:Set the transmission speed by writing to the UBRR0 register.Enable the transmitter 
hardware via the UCSR0B register.Create a function to wait for the transmit buffer (UCSR0A) to clear before writing 
characters to UDR0.ADC (Sensor) Configuration:Configure ADMUX to select the analog pin and voltage reference.Use ADCSRA 
to trigger analog-to-digital conversions and read the sensor value.2. The Automation MakefileSave this as Makefile 
alongside your firmware code to compile and flash with a single make command:makefileMCU = atmega328

F_CPU = 16000000UL
PORT = /dev/ttyACM0
BAUD = 115200

default:
	avr-gcc -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c main.c -o main.o
	avr-gcc -mmcu=$(MCU) -o main.elf main.o
	avr-objcopy -O ihex -R .eeprom main.elf main.hex
	avrdude -F -V -c arduino -p m328p -P $(PORT) -b $(BAUD) -U flash:w:main.hex

clean:
	rm -f *.o *.elf *.hex
Use code with caution.🖥️ Phase 3: The PC Desktop App (GTK4 & C)1. Host OS Data ExtractionCPU Parsing: Write standard C 
file parsing functions (fopen, fgets) to read /proc/stat. Calculate CPU usage by comparing delta total time versus delta 
idle time.RAM Parsing: Read /proc/meminfo to extract total and free memory variables.2. Non-Blocking Serial ReaderThe 
Trap: Avoid blocking functions like fscanf or read on the /dev/ttyACM0 file inside the main loop, as they freeze the
 interface.The Fix: Use GLib's GIOChannel paired with g_io_add_watch() to register a callback function that triggers 
 automatically only when the ATmega pushes new bytes into the buffer.3. The GTK4 Dynamic UI LoopAsynchronous Updates:
  Use g_timeout_add_seconds() to set up a regular callback interval (e.g., every 1 second).UI Elements:GtkLabel: 
  Displays numeric text strings.GtkProgressBar or GtkLevelBar: Visual scales bound directly to your system capacity 
  percentages.📋 Recommended Implementation OrderBlink an LED on the board using the bare-metal toolchain to confirm 
  you can flash binaries successfully.Read /proc/stat in a tiny standalone terminal C program to practice the CPU 
  calculation math.Write the ATmega serial output code and verify you can view raw text data streaming into your Linux 
  terminal using cat /dev/ttyACM0.Assemble the GTK4 App Layout, integrating the timer loop and serial watch logic to 
  update the widgets smoothly.When you are ready to start hacking on the code, let me know if you want to kick off with 
  the firmware side (LED blink / USART) or the GTK4 side (reading system stats)!