﻿<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# Versal Custom Thin Platform Extensible System
This tutorial describes a Versal VCK190 System Example Design based on a thin custom platform (minimal clocks and AXI exposed to PL) including HLS/RTL kernels and an AI Engine kernel using a full Makefile build-flow for Vivado™/Petalinux/Vitis 2021.2.

## Getting Started
### Build-flow
The Versal VCK190 System Example Design full Makefile build-flow builds the whole project in the following order:
```
  1. version_check: Checks if the Vivado, Petalinux and Vitis tools are setup and if the versions are 2021.2
  2. board_repo:    Downloads the board files for pre-production and es1 from the Xilinx GitHub 
  3. xsa:           Building the thin platform xsa (only pre-synth)
  4. petalinux:     Building Petalinux and sysroot
  5. xpfm:          Building the Vitis Platform
  6. bif:           Copy over necessary petalinux files to the generated software platform (required by the Vitis packager)
  7. ip:            Building Ai Engine graph(s) towards libadf.a and compiling hls/rtl kernels to *.xo
  8. ps_apps:       Building all XRT-based PS applications
  9. vitis:         Linking all kernels in the thin platform and packaging all necessary boot/run files
```
### Build-flow Dependencies
The following diagram explains the build-flow dependencies.

**Notes:**
 - The diagram should be read from right to left.
 - The diagram is for illustration only. The actual build-flow is more sequential.
<img src="./documentation/readme_files/Design_dependencies.svg">

### Build & Prerequisites
In the `[project-root]` you can start the full build with `make all` **after** taking following prerequisites into account:
  - **Before starting the build, please correctly setup the 2021.2 version of Vivado, Petalinux and Vitis**
    - If the tools are not setup correctly the build will stop with an ERROR message telling you what is not correctly setup!
    - Be sure that the 2021.2 Y2K22 patch is installed (This is not verified)! 
      - More info can be found here: https://support.xilinx.com/s/article/76960?language=en_US
  - Everything is in the GitHub repository; no extra files are needed. Although some are downloaded from GitHub.
  - `[project-root]/Makefile`: `export DEVICE_NAME := xcvc1902-vsva2197-2MP-e-S` for pre-production board version (default); **change it to `export DEVICE_NAME := xcvc1902-vsva2197-2MP-e-S-es1` for es1 board version**.
  - `[project-root]/Makefile`: `export TARGET := hw` for targetting a VCK190 (default); **change it to `export TARGET := hw_emu` to target hardware emulation**.
    - The build flow supports both TARGET's in the same `[project-root]`; but you need to execute them yourself the one after the other if you need both results!
    - Some generated directories are depending on the TARGET and are further shown as `[dir]_${TARGET}`.
  - `[project-root]/Makefile`: `export ILA_EN := 0` for disabling the ILA (default); **change it to `export ILA_EN := 1` to enable the ILA**.
  - `[project-root]/petalinux/Makefile`: the generated tmp-dir ends up in `/tmp`. 
    - If you want to place it somewhere else you need to add `--tmpdir [your_tmp_dir]` at the end of the `petalinux-create` line in the `[project-root]/petalinux/Makefile`. 
      - Be aware that `[your_tmp_dir]` may **NOT** be located on an NFS-drive!
  - End result: 
    - `export TARGET := hw`: `[project-root]/package_output_hw/sd_card/*` can be used for FAT-32 SD-card (partition); or `[project-root]/package_output_hw/sd_card.img` can be used.
    - `export TARGET := hw_emu`: `[project-root]/package_output_hw_emu/launch_hw_emu.sh` can be used to launch the hardware emulation.

## More In-Depth
The following explains the different sub-build steps. Click on each item for more detailed information.  
Each step is sequential (in the order listed - by the `[project-root]/Makefile`): 

<details>
 <summary> make version_check </summary>
 
 -  Checks if the Vivado, Petalinux and Vitis tools are setup and if the versions are 2021.2.
 
</details>
<details>
 <summary> make platform/hw/board_repo </summary>
 
 - Downloads all pre-production and es1 board files from the Xilinx GitHub.
 
</details>
<details>
 <summary> make xsa -C platform </summary>
 
`[project-root]/platform` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The platform xsa/xpfm Makefile                                  
| hw/*                | The hardware platform Makefile and sources

 - Builds the output file needed for Petalinux and Vitis software platform creation -> `[project-root]/platform/hw/build/vck190_thin.xsa`.
 - After this step you could open the platform blockdesign in Vivado for review
 
</details>
<details>
  <summary> make all -C petalinux </summary>

`[project-root]/petalinux` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The Petalinux Makefile                                  
| src/config          | A script used to exchange/add petalinux configuration items
| src/device-tree/*   | Some device-tree changes needed for VCK190              
| src/boot_custom.bif | bif file needed to have a correct BOOT.BIN in the Vitis packager   

 - Builds all required Petalinux images which end up in `[project-root]/petalinux/linux/images/linux`.
 - It also builds a `sysroot` which ends up in `[project-root]/petalinux/sysroot` (needed for `[project-root]/ps_apps` build).
 
</details>
<details>
  <summary> make xpfm -C platform </summary>

`[project-root]/platform` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The platform xsa/xpfm Makefile                                  
| sw/*                | The Vitis platform Makefile and sources

 - Builds the output files needed for ip and Vitis -> `[project-root]/platform/sw/build/vck190_thin/export/vck190_thin/vck190_thin.xpfm` and some generated subfolders.
 
</details>
<details>
  <summary> make bif </summary>

 - Copies over some Petalinux-generated files to the software platform. These are necessary for a correct Vitis-build.
 
</details>
<details>
  <summary> make all -C ip </summary>
 
`[project-root]/ip` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|--------------------------------------------------------------
| Makefile            | The ip generic Makefile; it automatically searches for sub-projects to build                             
| aie/*               | aie "datamover" kernel Makefile and sources
| counter/*           | free-running RTL "counter" kernel Makefile and sources that feeds the aie "datamover" kernel
| subtractor/*        | Managed RTL "subtractor" kernel Makefile and sources that measures the delay between the counter-input and the aie-output
| vadd/*              | XRT-controlled HLS "vadd" kernel Makefile and sources

 - Builds the output files needed for Vitis linker -> `[project-root]/ip/aie/libadf.a` and `[project-root]/ip/xo_${TARGET}/*.xo`
 - Kernel structure/flow:
    - vadd is a separate kernel
    - counter -> aie "datamover" -> subtractor
    
</details>
<details>
  <summary> make all -C ps_apps </summary>

`[project-root]/ps_apps` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|---------------------------------------------------------
| Makefile            | The ps_apps generic Makefile; it automatically searches for sub-projects to build
| aie_dly_test/*      | PS XRT Application - using the native XRT API - Makefile and sources that measures the delay between counter-input and aie-output
| vadd_cpp/*          | PS XRT Application - using the native XRT API - Makefile and sources that checks out the vadd kernel
| vadd_ocl/*          | PS XRT Application - using the opencl XRT API - Makefile and sources that checks out the vadd kernel

 - Builds the output files needed for vitis packager -> `[project-root]/ps_apps/exe/*.exe`
 
</details>
<details>
  <summary> make all -C vitis </summary>

`[project-root]/vitis` Directory/file structure:
| Directory/file      | Description                                             
| --------------------|---------------------------------------------------------
| Makefile            | The Vitis generic Makefile for linker and packager
| src/system.cfg      | Vitis connection and clock configuration needed for Vitis linker

 - Runs the Vitis linker and packager
 - The output of the Vitis packager ends up in `[project-root]/package_output_${TARGET}`
 - After this step you could open the full blockdesign (platform extended with all kernels) in Vivado for review
 
</details>

## Testing
### Running on a VCK190
  1. Prerequisite: Build was executed with `export TARGET := hw` and for your correct VCK190 version (pre-production or es1)
  2. Copy over the `[project-root]/package_output_hw/sd_card/*` to an SD-card or put the `[project-root]/package_output_hw/sd_card.img` on an SD-card.
  3. Put the SD-card in the VCK190 Versal SD-card slot (VCK190 top SD-card slot closest to the bracket).
  4. Connect the included USB-cable between the VCK190 (Middle bottom of the bracket) and a computer:
     - Usually you will see 3 serial ports in your device manager:
       - One for the ZU04 system controller device.
       - Two for Versal; however only one of the Versal serial ports are in use.
       - To see the serial ports, the VCK190 does not need to be powered-ON, the physical USB connection should be enough!
     - Connect to the serial port(s) by using a terminal emulator like Putty (Windows) with the following settings:
       - 115200 baud
       - 8 data bits
       - 1 stop bit
       - Parity none
       - Flow control XON/XOFF
     - Maybe for the first time open all 3 serial ports to see which one is the correct Versal serial port where you can follow the Versal-boot and interact later on.
  5. Power-UP:
     - It will first boot-up up the ZU04, next it will start the Versal boot. 
     - Only one of the Versal serial ports will give you the Linux command line prompt after booting.
     - No Password is needed for linux login.
  6. Continue to "Execution & Results"

### Running Hardware Emulation
  1. Prerequisite: Build was executed with `export TARGET := hw_emu`
  2. Execute `source ./launch_hw_emu.sh` in `[project-root]/package_output_hw_emu/`.
  3. The hardware emulation will start and boot-up the whole system and should give you the Linux command line prompt.
     - This can take some time!
     - No password is needed for linux login.
  4. Continue to "Execution & Results"

### Execution & Results
  - Execute the following after boot-up when you reached the Linux command line prompt:
    - In the logging below you find all results/responses that you should get after every Linux command line input you should give.
  
 ```
    root@linux:~# cd /media/sd-mmcblk0p1/
    root@linux:/media/sd-mmcblk0p1# ./vadd_cpp.exe a.xclbin
    Passed: auto my_device = xrt::device(0)
    Passed: auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
    Passed: auto my_vadd = xrt::kernel(my_device, xclbin_uuid, "vadd")
    VADD TEST PASSED
    root@linux:/media/sd-mmcblk0p1# ./vadd_ocl.exe a.xclbin
    Loading: 'a.xclbin'
    VADD TEST PASSED
    root@linux:/media/sd-mmcblk0p1# ./aie_dly_test.exe a.xclbin
    Initializing ADF API...
    Passed: auto my_device = xrt::device(0)
    Passed: auto xclbin_uuid = my_device.load_xclbin(a.xclbin)
    Passed: auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, "subtractor")
    Passed: auto my_graph  = xrt::graph(my_device, xclbin_uuid, "mygraph_top")
    Passed: my_graph.reset()
    Passed: my_graph.run()
    Poll subtractor register
      Value Reg0:  210
      Value Reg1:  11c
      Value Reg2:  172
      Value Reg3:  3a
    Poll subtractor register
      Value Reg0:  23e
      Value Reg1:  11a
      Value Reg2:  175
      Value Reg3:  3a
    ...
    Poll subtractor register
      Value Reg0:  1ec
      Value Reg1:  118
      Value Reg2:  173
      Value Reg3:  38
    Poll subtractor register
      Value Reg0:  246
      Value Reg1:  11a
      Value Reg2:  182
      Value Reg3:  36
    Passed: my_graph.end()
    root@linux:/media/sd-mmcblk0p1#
  ```

## Notes
  - xsa: CIPS settings are added manually; configured in the bd-files.
  - The example design is fully FAT-32 
    - if you like to use ext4 rootfs instead: 
      - petalinux already generates it.
      - You will need to copy it to the Vitis platform in the `[project-root]/Makefile` in the `bif` section.
      - The `v++ -p` command line in `[project-root]\vitis\Makefile` will need adaptations to be able to use ext4 rootfs instead of FAT-32.
  - `export ILA_EN := 1`
    - The ILA core connectivity is set up during v++ linking process loading the cfg file `[project-root]/vitis/src/ila_0_bd.cfg` and further configuration of ILA properties is managed in tcl file `[project-root]/vitis/src/ila_0_def.tcl`.
    - Using the configuration file `[project-root]/vitis/src/ila_0_bd.cfg` allows the designer to mark AXI port for debug nets to and from the AIE engine for analysis. 
    - After completing the linking process, the designer can verify conectivity and configuration of the ILA core in the generated block design in project `[project-root]/vitis/build_${TARGET}/_x/link/vivado/vpl/prj/prj.xpr`.
    - Once the build process is completed and petalinux boots on your board, it is required to manually set the path for probe file `[project-root]/package_output/probe_0.ltx` in the Vivado Hardware Manager to load the ILA core if this was enabled. 
    - A quick use case would be to validate the values of subtractor registers. After the probing file is loaded and the ILA is armed, re-running `./aie_dly_test.exe a.xclbin` will trigger the ILA capturing the signal values that should match those in the console.
  - Simulation is **NOT** part and **NOT** demonstrated in this Tutorial!
  
## Design Considerations
  Note: The **MUST**'s in below explanations are due to how the generic Makefiles are setup, and is **NOT** a Xilinx tools requirement!
  - `[project-root]/ps_apps`: PS applications can easily be added by adding a sub-project for each in `[project-root]/ps_apps/`.
    - Vitis will automatically package them and they will end up in `[project-root]/package_output_${TARGET}`.
    - The `[PS Application].exe` (extension **MUST** be .exe) **MUST** end up in the `[project-root]/ps_apps/exe` dir.
  - `[project-root]/ip`: Kernels can be added by just adding a sub-project in `[project-root]/ip`.
    - **You will need to update the `[project-root]/vitis/src/system.cfg` file to setup the correct connections/clocks.**
    - A `[kernel].xo` file **MUST** end up in the `[project-root]/ip/xo_${TARGET}` dir
    - An extra aie graph **MUST** be added in the `[project-root]/ip/aie` dir, the `[project-root]/ip/aie/Makefile` will need adaptations.

## References
The following documents provide supplemental information for this tutorial.

### [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/yii1603912637443.html)
Contains sections on how to develop AI Engine graphs, how to use the AI Engine compiler, AI Engine simulation, and performance analysis.

### [Xilinx Runtime (XRT) Architecture](https://xilinx.github.io/XRT/master/html/index.html)
The following are links to the XRT information used by this tutorial: 

* [XRT Documentation](https://xilinx.github.io/XRT/master/html/index.html): Explains general XRT API calls used in the PS Host Application. 

* [XRT Github Repo](https://github.com/Xilinx/XRT): Contains the XRT source code. 

* [XRT AIE API](https://github.com/Xilinx/XRT/blob/master/src/runtime_src/core/include/experimental/xrt_aie.h): Documents the AI Engine XRT API calls

### [Vitis Unified Software Development Platform 2021.2 Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/index.html)
The following are links to Vitis related information referenced in this tutorial:

* [Vitis Application Acceleration Development Flow Documentation](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/kme1569523964461.html)

* [Vitis Application Acceleration Development Flow Tutorials](https://github.com/Xilinx/Vitis-Tutorials)

* [Vitis HLS](https://www.xilinx.com/html_docs/xilinx2021_2/vitis_doc/irn1582730075765.html)

## Revision History
* October 2021 - Optimized AI Engine Datamovers + Added more clarifications in this README.md + Improved petalinux version check
* September 2021 - Initial Release

 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD106 | © Copyright 2021 Xilinx, Inc.</sup></p>
