include ../../../../hardware/$(ARCH)/make.rule

BUILD_DIR = ../../../../build
TARGET_DIR = $(BUILD_DIR)/rootfs

LDFLAGS = -L$(BUILD_DIR)/lib

CXXFLAGS += -I $(BUILD_DIR)/include

XCONSOLED_OBJS = xconsoled.o

XCONSOLED = $(TARGET_DIR)/drivers/xconsoled

$(XCONSOLED): $(XCONSOLED_OBJS) \
		$(EWOK_LIBC_A)  \
		$(BUILD_DIR)/lib/libsconf.a \
		$(BUILD_DIR)/lib/libfb.a \
		$(BUILD_DIR)/lib/libterminal.a \
		$(BUILD_DIR)/lib/libgterminal.a \
		$(BUILD_DIR)/lib/libtextview.a \
		$(BUILD_DIR)/lib/libgraph.a \
		$(BUILD_DIR)/lib/libupng.a \
		$(BUILD_DIR)/lib/libttf.a  \
		$(BUILD_DIR)/lib/libfont.a  \
		$(BUILD_DIR)/lib/libx.a  \
		$(BUILD_DIR)/lib/libx++.a  \
		$(BUILD_DIR)/lib/libcxx.a  \
		$(BUILD_DIR)/lib/libdisplay.a 
	$(LD) -Ttext=100 $(XCONSOLED_OBJS) -o $(XCONSOLED) $(LDFLAGS) -lfb -ldisplay -lx++ -lx -lcxx -lgterminal -lterminal -ltextview -lgraph -lbsp -lttf  -lfont -lupng -lsconf $(EWOK_LIBC)

clean:
	rm -f $(XCONSOLED_OBJS)
