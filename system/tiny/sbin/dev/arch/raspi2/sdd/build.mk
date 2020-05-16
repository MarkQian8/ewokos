RASPI2_SDD_OBJS = $(ROOT_DIR)/sbin/dev/arch/raspi2/sdd/sdd.o
RASPI2_SDD = $(TARGET_DIR)/$(ROOT_DIR)/sbin/dev/raspi2/sdd

PROGS += $(RASPI2_SDD)
CLEAN += $(RASPI2_SDD_OBJS)

$(RASPI2_SDD): $(RASPI2_SDD_OBJS) 
	$(LD) -Ttext=100 $(RASPI2_SDD_OBJS) -o $(RASPI2_SDD) $(LDFLAGS) -lewokc -lc
