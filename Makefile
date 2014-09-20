#
# Makefile for Invensense inv_mpu_iio device.
#

obj-$(CONFIG_INV_MPU_IIO) += inv_mpu_iio.o

inv_mpu_iio-objs := inv_mpu_core.o
inv_mpu_iio-objs += inv_mpu_ring.o
inv_mpu_iio-objs += inv_mpu_trigger.o
inv_mpu_iio-objs += inv_mpu_misc.o
inv_mpu_iio-objs += inv_mpu3050_iio.o
inv_mpu_iio-objs += dmpDefaultMPU6050.o
inv_mpu_iio-objs += inv_slave_compass.o
inv_mpu_iio-objs += inv_slave_pressure.o

CFLAGS_inv_mpu_core.o      += -Idrivers/staging/iio
CFLAGS_inv_mpu_ring.o      += -Idrivers/staging/iio
CFLAGS_inv_mpu_trigger.o   += -Idrivers/staging/iio
CFLAGS_inv_mpu_misc.o      += -Idrivers/staging/iio
CFLAGS_inv_mpu3050_iio.o   += -Idrivers/staging/iio
CFLAGS_dmpDefaultMPU6050.o += -Idrivers/staging/iio
CFLAGS_inv_slave_compass.o   += -Idrivers/staging/iio
CFLAGS_inv_slave_pressure.o   += -Idrivers/staging/iio

# the Bosch BMA250 driver is added to the inv-mpu device driver because it
# must be connected to an MPU3050 device on the secondary slave bus.
ifeq ($(CONFIG_INV_IIO_MPU3050_ACCEL_SLAVE_BMA250), y)
inv_mpu_iio-objs += inv_slave_bma250.o
CFLAGS_inv_slave_bma250.o   += -Idrivers/staging/iio
endif

