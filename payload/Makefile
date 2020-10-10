include ../fusee-launcher/Makefile

all: uart_payload_n7.bin uart_payload_ouya.bin

CFLAGS += -O0

# if you actually want to build these, just cd ../fusee-launcher && make
intermezzo.bin dump-sbk-via-usb.bin: ;

uart_payload_%.elf: uart_payload_%.o
	$(LD) -T uart_payload.lds --defsym LOAD_ADDR=$(ENTRY_POINT_ADDRESS) $(LDFLAGS) $^ -o $@

uart_payload_%.o: uart_payload.c
	$(CC) $(CFLAGS) $(DEFINES) $(if $(filter $*, n7),-DUARTA,-DUARTD) $< -c -o $@
