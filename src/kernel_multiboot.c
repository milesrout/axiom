#include <stddef.h>
#include <stdint.h>

#include "term.h"

struct multiboot_data {
	uint32_t flags;
};

enum multiboot_info_flags {
	MB_INFO_MEM_LOWER_PRESENT = 0x01,
	MB_INFO_BOOT_DEVICE_PRESENT = 0x02,
	MB_INFO_CMDLINE_PRESENT = 0x04,
	MB_INFO_MODS_PRESENT = 0x08,
	MB_INFO_SYMS_PRESENT_4 = 0x10,
	MB_INFO_SYMS_PRESENT_5 = 0x20,
	MB_INFO_MMAP_PRESENT = 0x40,
	MB_INFO_DRIVES_PRESENT = 0x80,
	MB_INFO_CONFIG_TABLE_PRESENT = 0x100,
	MB_INFO_BOOTLOADER_NAME_PRESENT = 0x200,
	MB_INFO_APM_PRESENT = 0x400,
	MB_INFO_VBE_PRESENT = 0x800
};

void multiboot_init(struct multiboot_data *mb_info)
{
	terminal_writestring("\n----------\n");
	terminal_writestring("Finding multiboot info\n");

	terminal_writestring("\tMultiboot info is at address: ");
	terminal_putptr(mb_info);
	terminal_writestring(".\n");

	terminal_writestring("\tMultiboot flags: ");
	terminal_putbin_u32(mb_info->flags);
	terminal_writestring(".\n");

	terminal_writestring("\t\tmem_* fields present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_MEM_LOWER_PRESENT) != 0);
	terminal_writestring(".\n\t\tboot_device field present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_BOOT_DEVICE_PRESENT) != 0);
	terminal_writestring(".\n\t\tcmdline field present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_CMDLINE_PRESENT) != 0);
	terminal_writestring(".\n\t\tmods_* fields present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_MODS_PRESENT) != 0);

	terminal_writestring(".\n\t\tflags[4] present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_SYMS_PRESENT_4) != 0);
	terminal_writestring(".\n\t\tflags[5] present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_SYMS_PRESENT_5) != 0);
	terminal_writestring(".\n\t\tsyms present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_SYMS_PRESENT_4) != 0
	                     || (mb_info->flags & MB_INFO_SYMS_PRESENT_5) != 0);

	terminal_writestring(".\n\t\tmmap_* fields present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_MMAP_PRESENT) != 0);
	terminal_writestring(".\n\t\tdrives_* fields present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_DRIVES_PRESENT) != 0);
	terminal_writestring(".\n\t\tconfig_table present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_CONFIG_TABLE_PRESENT) != 0);
	terminal_writestring(".\n\t\tapm_table present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_APM_PRESENT) != 0);
	terminal_writestring(".\n\t\tvbe_* fields present: ");
	terminal_putint_u32((mb_info->flags & MB_INFO_VBE_PRESENT) != 0);

	terminal_writestring("----------\n");
}
