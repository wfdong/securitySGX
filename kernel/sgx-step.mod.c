#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xdd8f8694, "module_layout" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0xc1ec8b42, "register_kretprobe" },
	{ 0xdd57a04f, "put_user_pages" },
	{ 0x5ab904eb, "pv_ops" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x978f9593, "misc_register" },
	{ 0x56b1771b, "current_task" },
	{ 0xc5850110, "printk" },
	{ 0xacf3ff14, "vmap" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0xe7b00dfb, "__x86_indirect_thunk_r13" },
	{ 0xe6a2aaf8, "unregister_kretprobe" },
	{ 0x1d19f77b, "physical_mask" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x37a0cba, "kfree" },
	{ 0x94961283, "vunmap" },
	{ 0x69acdf38, "memcpy" },
	{ 0x53c47c53, "apic" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xdd02ac5a, "misc_deregister" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xfb8ca8d6, "get_user_pages_fast" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E2A1E623124DA6ADB09FC8C");
