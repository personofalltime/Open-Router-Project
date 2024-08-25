# Open-Router-Project
A mostly 3D printed CNC Router + Firmware (for any arduino microcontroller) that I designed and developed myself!


# What is it?
Open-Router is a Cheap, open-source CNC Router that I designed and developed over the past month. I wrote some easy to use arduino firmware with a serial gcode interpreter (in theory, is compatible with most software for streaming gcode) that you can use alongside this machine, or most other CNC machines! I'm working on expanding support for even more gcode commands!

I started this project as a massive side project, when I wanted to work on some other larger projects at home (an electric motorbike), but soon realised that in order to be able to cut metal components to the precision I wanted, I would need some kind of CNC machine. All of the existing packages I found were too expensive, and massive overkill for what I needed, so I set out to make my own system! I went through more than 6 designs, with the final 3 looking similar to the one I settled with finally. This final version uses a ganged Y axis, and is built on a stable frame made of aluminium extrusion and held square with linear rails. These, along with the use of leadscrews, help to make sure that I can get high precision parts without expensive calibration tools. It uses bog-standard NEMA 17 motors since they were very cheap off of Aliexpress (and are widely compatible with a lot of stepper drivers) while also being sufficiently high quality for my application (they are used in eighth step mode for this, but this can be changed to alter the speed or precision). I used GT2 timing belts for driving the Z-axis since its ubiquitously used in the 3D printing world and i had some spare from a printer modification. 

Because of some oversights early in the development, and my not wanting to spend more money, the router has a smaller than 300 by 300mm cutting area, being around 280 by 240mm, with 60mm of Z-travel. This is more than enough for my application, but can easily be expanded by changing some extrusion lengths

Even though the router was made for a specific purpose, I only wrote the firmware just because I wanted to - there are plenty of other available software packages that you could set up with this system (I.E. using the Genmitsu GRBL control board would reduce a lot of the effort and cut the price down by over £20). But, I really wanted to experience writing some lower-level code. My next upgrade for this is creating a custom control board with an STM32 chip and integrated drivers and ESCs, but that'll take me quite a long time (because of university applications...) 

# How expensive is it
One of my biggest goals while making this was to keep the cost low while retaining functionality, and I would definitely say that I achieved this. It's quite cheap, especially compared to off-the-shelf solutions!
  ![image](https://github.com/user-attachments/assets/0cb5ddbd-e69c-4d43-a967-553133b2c64e)
Sub-£200!!
It can come out to being a lot cheaper given the ubiquity of most of these parts, and my overestimating of filament used (I used just over a kilogram for 3 full prototypes!)

# Setup instructions
Follow assembly as shown in the full CAD model, available under the CAD folder. I've also provided pictures from every orthogonal direction, so it should be simple to assemble with those two resources combined. 

# Functionality
I haven't managed to test this yet, due to lead times on some parts, but I am expecting to be able to cut aluminium and possibly thin steel with this! MDF and Plywood should definitely be cuttable with this system.

# Images
![image](https://github.com/user-attachments/assets/29695eac-b4f5-493b-a868-408705b583a7)
![image](https://github.com/user-attachments/assets/ee8e68f6-4f08-4a24-9c22-29e7b1e00c33)


# Previous Versions

![image](https://github.com/user-attachments/assets/0f805b34-063d-40fe-9609-c1ede139d2b6)
![image](https://github.com/user-attachments/assets/777aaba9-9f9a-4d34-8164-72a7d5f2135a)
![image](https://github.com/user-attachments/assets/b3980f41-9089-4c49-be3a-071128b08cc8)

