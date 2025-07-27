/*-------------------------------------------------------------------------
 * Filename:      nwy_open_app.lds
 *-----------------------------------------------------------------------*/
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    APP_RAM (rwx)  : ORIGIN = 0x7e040800, LENGTH = (768*1024)
}

SECTIONS
{
	nwy_bin_header : 
	{ 
		KEEP(*(nwy_bin_header))
	} > APP_RAM
	.text ALIGN(4): 
	{ 
	    KEEP(*(nwy_open_asm))
	    *(.text*)
	} > APP_RAM
	.rodata ALIGN(4): 
	{
	    *(.rodata*)
	} > APP_RAM
	nwy_open_api_import ALIGN(4):
	{
		PROVIDE_HIDDEN (nwy_open_api_import$$Base = .);
		KEEP(*(nwy_open_api_import))
		PROVIDE_HIDDEN (nwy_open_api_import$$End = .);
    } > APP_RAM
	nwy_open_api_import$$Length = SIZEOF(nwy_open_api_import);
	.data ALIGN(4): 
	{
		*(.data*)
	} > APP_RAM
	.bss ALIGN(4):
	{
	__bss_start = .;
		*(.bss*)
	__bss_end = .;
	end = .;
	} > APP_RAM
}

