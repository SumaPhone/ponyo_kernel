#
# Makefile for miscellaneous I2C chip drivers.
#
# Do not add new drivers to this directory! It is DEPRECATED.
#
# Device drivers are better grouped according to the functionality they
# implement rather than to the bus they are connected to. In particular:
# * Hardware monitoring chip drivers go to drivers/hwmon
# * RTC chip drivers go to drivers/rtc
# * I/O expander drivers go to drivers/gpio
#

obj-$(CONFIG_SENSORS_MT9T013)	+= mt9t013.o

# + qiukj add for bma222
obj-$(CONFIG_BOSCH_BMA222)      += bma222_driver.o bma222.o
# - qiukj add for bma222

# + qiukj add for bma250
obj-$(CONFIG_BOSCH_BMA222)      += bma250_driver.o
# - qiukj add for bma250

ifeq ($(CONFIG_I2C_DEBUG_CHIP),y)
EXTRA_CFLAGS += -DDEBUG
endif

# + qiukj add for bma222
ifeq ($(CONFIG_BOSCH_BMA222_USE_SMBUS),y)
EXTRA_CFLAGS += -DBMA222_SMBUS
endif
ifeq ($(CONFIG_BOSCH_BMA222_WITH_DEBUG),y)
EXTRA_CFLAGS += -DBMA222_DEBUG
endif
ifeq ($(CONFIG_BOSCH_BMA222_HAS_EARLYSUSPEND),y)
EXTRA_CFLAGS += -DBMA222_HAS_EARLYSUSPEND
endif
# - qiukj add for bma222

# + qiukj add for bma250
ifeq ($(CONFIG_BOSCH_BMA250_USE_SMBUS),y)
EXTRA_CFLAGS += -DBMA250_SMBUS
endif
ifeq ($(CONFIG_BOSCH_BMA250_WITH_DEBUG),y)
EXTRA_CFLAGS += -DBMA250_DEBUG
endif
ifeq ($(CONFIG_BOSCH_BMA250_HAS_EARLYSUSPEND),y)
EXTRA_CFLAGS += -DBMA250_HAS_EARLYSUSPEND
endif
ifeq ($(CONFIG_BOSCH_BMA250_ENABLE_IRQ),y)
EXTRA_CFLAGS += -DBMA250_ENABLE_IRQ
endif
# - qiukj add for bma250

ifeq ($(CONFIG_I2C_DEBUG_CHIP),y)
EXTRA_CFLAGS += -DDEBUG
endif

