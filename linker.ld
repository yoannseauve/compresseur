MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 16K
  RAM (rxw)  : ORIGIN = 0x20000000, LENGTH = 4K
}

ENTRY(_reset_handler);
_Min_Leftover_RAM = 0x400; /* 1k left for stack */

SECTIONS
{
	.vector_table :
	{
		LONG(ORIGIN(RAM)+LENGTH(RAM));
		KEEP(*(.vector_table));
	} > FLASH

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text.*)
		. = ALIGN(4);
	} > FLASH

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
	} > FLASH
	
	_sidata = LOADADDR(.data);
	.data :
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data)
		*(.data.*)
		_edata = .;
	} > RAM AT> FLASH
	
	.bss :
	{
		. = ALIGN(4);
		_sbss = .;
		*(.bss)
		*(.bss.*)
		_ebss = .;
	} > RAM
	
	.leftStack :
	{
		. = . + _Min_Leftover_RAM;
	} > RAM
}
