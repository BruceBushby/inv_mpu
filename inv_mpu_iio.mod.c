#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:itg3500");
MODULE_ALIAS("i2c:mpu3050");
MODULE_ALIAS("i2c:mpu6050");
MODULE_ALIAS("i2c:mpu9150");
MODULE_ALIAS("i2c:mpu6500");
MODULE_ALIAS("i2c:mpu9250");
MODULE_ALIAS("i2c:mpu6xxx");
MODULE_ALIAS("i2c:mpu9350");
MODULE_ALIAS("i2c:mpu6515");
