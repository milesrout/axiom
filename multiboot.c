#include <stddef.h>
#include <stdint.h>

#include "term.h"

static void log_fields_present();

struct multiboot_info {
	uint32_t flags;

	/* only valid if (flags & MB_INFO_MEM_PRESENT) != 0 */
	uint32_t mem_lower;
	uint32_t mem_upper;

	/* only valid if (flags & MB_INFO_BOOT_DEVICE_PRESENT) != 0 */
	uint32_t boot_device;

	/* only valid if (flags & MB_CMDLINE_PRESENT) != 0 */
	uint32_t cmdline;

	/* only valid if (flags & MB_MODS_PRESENT) != 0 */
	uint32_t mods_count;
	uint32_t mods_addr;
};

struct multiboot_mods {
	uint32_t mod_start;
	uint32_t mod_end;
	uint32_t mod_cmdline;

	/* must be 0 */
	uint32_t pad;
};

extern struct multiboot_info *_multiboot_info;

enum multiboot_info_flags {
	MB_INFO_MEM_PRESENT = 0x01,
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


void multiboot_init()
{
	uint32_t mods_present;

	terminal_put_str("\n----------\n");
	terminal_put_str("Finding multiboot info\n");

	terminal_put_str("\tMultiboot info is at address: ");
	terminal_put_ptr(_multiboot_info);
	terminal_put_str(".\n");

	if (0) {
		log_fields_present();
	}

	mods_present = _multiboot_info->flags & MB_INFO_MODS_PRESENT;
	if (!mods_present) {
		terminal_put_str("No mods present!");
		__asm__ __volatile__("cli\n\thlt" : :);
	}
	terminal_put_str("Mods present: ");
	terminal_put_u32(_multiboot_info->mods_count);
	terminal_put_str(".\n");
}

static void log_fields_present()
{
	terminal_put_str("\tMultiboot flags: ");
	terminal_put_u32b(_multiboot_info->flags);
	terminal_put_str(".\n");

	terminal_put_str("\t\tmem_* fields present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_MEM_PRESENT) != 0);
	terminal_put_str(".\n\t\tboot_device field present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_BOOT_DEVICE_PRESENT) != 0);
	terminal_put_str(".\n\t\tcmdline field present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_CMDLINE_PRESENT) != 0);
	terminal_put_str(".\n\t\tmods_* fields present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_MODS_PRESENT) != 0);

	terminal_put_str(".\n\t\tflags[4] present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_SYMS_PRESENT_4) != 0);
	terminal_put_str(".\n\t\tflags[5] present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_SYMS_PRESENT_5) != 0);
	terminal_put_str(".\n\t\tsyms present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_SYMS_PRESENT_4) != 0
	                     || (_multiboot_info->flags & MB_INFO_SYMS_PRESENT_5) != 0);

	terminal_put_str(".\n\t\tmmap_* fields present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_MMAP_PRESENT) != 0);
	terminal_put_str(".\n\t\tdrives_* fields present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_DRIVES_PRESENT) != 0);
	terminal_put_str(".\n\t\tconfig_table present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_CONFIG_TABLE_PRESENT) != 0);
	terminal_put_str(".\n\t\tapm_table present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_APM_PRESENT) != 0);
	terminal_put_str(".\n\t\tvbe_* fields present: ");
	terminal_put_u32((_multiboot_info->flags & MB_INFO_VBE_PRESENT) != 0);

	terminal_put_str("----------\n");
}
