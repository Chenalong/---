OUTPUT_FORMAT("elf32-arcal")

SECTIONS
{
  _etext  = 0;
  _end_sys_data = 0;
/* for code section */

  _stack = 0x0c001FFC;			/* sram highest addr */

  . = 0x00008000;		/* flash lowest */
  _text = .;			/* Text and read-only data */
  .text : AT (0x00008000){
	*(.sys_text.head);
	*(.sys_text);
	*(.text.head);
	*(.text);
	_etext = ABSOLUTE (.) ;
	} 

  .rodata _etext : AT (_etext){ *(.rodata); _erodata = ABSOLUTE (.); }
  
  
  . = 0x0c000020;		/* sram base */ 
  .sys_data 0x0c000020 : AT(_erodata) 
			{ 
				*(.sys_data);
				 _esys_data = ABSOLUTE (.);
				 _end_sys_data = ABSOLUTE (.);
			}   /* Sys_data */ 

  
  .data _end_sys_data :  AT(_erodata + SIZEOF(.sys_data))
			{
				 *(.data);
			 	_edata = ABSOLUTE (.);
			}	/* Data */

 .bss  :		{ 
	  			 __bss_start = . ;
	   			*(.bss) 
			        *(.COMMON)
	   			__bss_end = ABSOLUTE (.);
        		} 
   _end = ADDR(.bss) + SIZEOF (.bss);
   end = ADDR(.bss) + SIZEOF (.bss);
  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
}





