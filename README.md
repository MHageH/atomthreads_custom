# Atomthreads RTOS
---------------------------------------------------------------------------

- Library:      Atomthreads
- Author:       Kelvin Lawson <info@atomthreads.com>
- Website:      http://atomthreads.com
- License:      BSD Revised

---------------------------------------------------------------------------

Atomthreads is a free RTOS for embedded systems, released under the
flexible, open source BSD license and is free to use for commercial or
educational purposes without restriction.

It is targeted at systems that need only a lightweight scheduler and the
usual RTOS primitives. No file system, IP stack or device drivers are
included, but developers can bolt on their own as required. Atomthreads
will always be a small number of C files which are easy to port to any
platforms that require threading by adding a simple
architecture-specific file.

---------------------------------------------------------------------------

DOCUMENTATION:

All documentation is contained within the source files, commented using
Doxygen markup. Pre-generated documentation can be accessed at
http://atomthreads.com.

See also the README file contained within each folder of the source tree.

---------------------------------------------------------------------------

GETTING STARTED:

Building of the sources is carried out from the ports tree. For example to 
make a software build for the Cortex-m architecture 
see ports/cortex-m/README.

---------------------------------------------------------------------------

SOURCE TREE:

 * kernel        Core kernel sources
 * tests         Automated test suite
 * ports         CPU architecture ports

---------------------------------------------------------------------------

# Modifications

- No modification to the kernel code itself.
- Removed nanolib support since it made conflicts on my machine. Check the
make file in ports/cortex-m to see if it's necessary for your application.

I added the STM32F4-discovery and STM32vl-discovery boards and removed all 
the others, even other architectures

To compile :
- cd ports/cortex-m
- make -j(number_of_cores) BOARD=stm32f4-discovery OR
- make -j(number_of_cores) BOARD=stm32vl-discovery

Then :
- cd build
- st-flash write <name>.bin 0x8000000

(This assumes that you have installed st-link and summon-arm-toolchain, if
not, check my c_uart_interface for instructions on doing it)

You can find Graphs in Global_graph that explains the functionnality of the
scheduler

