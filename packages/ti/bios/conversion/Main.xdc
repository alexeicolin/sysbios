/*
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 *  ======== Main ========
 *  Command-line conversion tool
 *
 *  This command converts a DSP/BIOS 5 Tconf script (*.tcf file) into a
 *  SYS/BIOS 6 configuration.

 *  This generated SYS/BIOS 6 configuration consists of a RTSC configuration
 *  script (*.cfg file) and a platform package.  The platform package will
 *  define the program's memory map, as translated from the *.tcf
 *  file, and the *.cfg will contain all other settings from the original *.tcf
 *  script.
 * 
 *  Once the conversion tool is run successfully, the generated RTSC
 *  configuration file (*.cfg) and generated platform package may be brought 
 *  into the program's CCSv4 project in order to build.
 *
 *  The generated platform package will be placed into the repository specified
 *  using the --pr option, and will be named from the --pn option.
 *
 *  For more information and examples on the conversion tool, please refer to
 *  the document "Bios_Legacy_App_Note.pdf", found in your SYS/BIOS
 *  installation directory.
 *
 *  @a(USAGE)
 *  @p(code)
 *    xs ti.bios.conversion [-v|--help] [-i] [-c outfile.cfg] 
 *            [--pr package repository] [--pn package name] infile.tcf
 *  @p
 *
 *  @a(INPUTS)
 *  @p(dlist)
 *    - `infile.tcf`
 *      A user-supplied Tconf configuration script. If the extension is not
 *      specified, '.tcf' is assumed.
 *  @p
 *
 *  @a(OUTPUTS)
 *  @p(dlist)
 *    - `outfile.cfg`
 *      A file containing a RTSC configuration script. This script looks
 *      syntactically
 *      like a Tconf script, but is executed within the XDCtools object
 *      model.
 *    - `platform package`
 *      A platform package containing the description of the platform derived
 *      from the Tconf
 *      script 'infile.tcf'. This package should be used in combination with the
 *      generated RTSC configuration script `outfile.cfg` to configure and build
 *      the program in CCSv4.
 *
 *      The name of the platform package is named 'plat' by default, unless a
 *      different name is specified using '-pn' option.
 *  @p
 *
 *  For example:
 *  @p(code)
 *     xs ti.bios.conversion -c app.cfg --pr C:/myRepository 
 *                          --pn myPlatform app.tcf
 *  @p
 */
metaonly module Main inherits xdc.tools.ICmd {

    /*!
     * usage help message
     */
    override config String usage[] = [
        '[-v] [-i]',
        '[-c outfile.cfg] [--pr package repository] [--pn package name] infile.tcf'
    ];

instance:

    /*!
     * name of the output XDC script
     */
    @CommandOption("c")
    config String outputFile = null;

    /*!
     * name of the platform instance
     */
    @CommandOption("pn")
    config String platformName = "plat";

    /*!
     * full path to the repository in which to store the generated platform
     * package.
     */
    @CommandOption("pr")
    config String platformRepository = ".";

    /*!
     * Java properties
     */
    @CommandOption("D")
    config String defines[] = [];

    /*!
     * show details during build
     */
    @CommandOption("v")
    config Bool verbose = false;

    /*!
     *  content of imported files is copied into the output file
     *
     *  This flag causes any file imported with `utils.importFile` to
     *  be copied into the new file. Without this flag, the line importing
     *  a file is left as it is.
     */
    @CommandOption("i")
    config Bool includeImports = false;

    /*!
     * Run the actual conversion code.
     */
    Int convert(String infile);
}


