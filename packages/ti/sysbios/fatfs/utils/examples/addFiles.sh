#
# Copyright (c) 2012, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  
#  * Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
# ======== addOutFiles.sh ========
#
# This script demonstrates how to use the FatFS Linux utilities to create a RAM
# disk image, copy 3 target executables into it and then save the image into a
# C array. The C array can be built into a BIOS application and loaded as a RAM
# disk, which will allow that app to access the 3 executables stored there.
#
#!/bin/sh

echo ""
echo "creating RAM disk image 'imageWithFiles.bin, size: 98302"
../bin/mkimage.x86U imageWithFiles.bin 131072
echo ""

echo "copying file1.txt into *.bin file ..."
../bin/cptoimage.x86U imageWithFiles.bin file1.txt file1.txt
echo "... done."
echo ""

echo "copying file2.txt into *.bin file ..."
../bin/cptoimage.x86U imageWithFiles.bin file2.txt file2.txt
echo "... done."
echo ""

echo "************************************"
echo "printing contents of RAM disk image:"
echo "************************************"
../bin/lsimage.x86U imageWithFiles.bin
echo ""

echo "converting contents of RAM disk binary file 'imageWithFiles.bin' into C array ramDiskArray[]:"
../bin/imagetosrc.x86U imageWithFiles.bin ramdiskWithFiles.c ramDiskArray
echo "... done."
