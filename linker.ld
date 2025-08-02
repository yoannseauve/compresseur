MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 15K
  FLASH_MEM (rw) : ORIGIN = 0x08003C00, LENGTH = 1K
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
	
	_start_stored_settings  = LOADADDR(.stored_settings);
	.stored_settings :
	{
		. = ALIGN(4);
		_sstored_settings = .;
		*(.stored_settings)
		_estored_settings = .;
	} > RAM AT> FLASH_MEM

	.leftStack :
	{
		. = . + _Min_Leftover_RAM;
	} > RAM

}
