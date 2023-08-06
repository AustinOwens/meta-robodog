obj-m += axi2spi_periph.o

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

modules_install:
	install -D -m 644 axi2spi_periph.ko $(KERNEL_OUTPUT)/axi2spi_periph.ko

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean

