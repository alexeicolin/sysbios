
function init() {
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    switch (prog.build.target.name) {
        case "C28_large":
        case "C28_float":
	    Program.stack = 0x2;
	    Program.argSize = 8;

    	    Program.sectMap[".stack"] = "MSARAM";
    	    Program.sectMap[".ebss"] = "L03SARAM";
    	    Program.sectMap[".text"] = "L47SARAM";
    	    Program.sectMap[".const"] = "L47SARAM";
    	    Program.sectMap[".args"] = "L47SARAM";
    	    Program.sectMap[".cio"] = "L47SARAM";
    	    Program.sectMap[".reset"] = "L47SARAM";
    	    Program.sectMap[".switch"] = "L47SARAM";
    	    Program.sectMap[".econst"] = "L47SARAM";
    	    Program.sectMap[".xdc_noload"] = "SRAM";
    	    
	    break;
        default:
    	    break;
    }
}
