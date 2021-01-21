SDCC        ?= sdcc
PROG    	?= easypdkprog

#setup target PFS173
ICTYPE      = pdk15
ICNAME      = PFS173

OUTDIR=build
$(shell mkdir -p $(OUTDIR))

TARGET=$(OUTDIR)/MCxxxx.$(ICNAME).ihx
SRCS := MCxxxx.c

CFLAGS =-m$(ICTYPE) -D$(ICNAME)
LDFLAGS=-m$(ICTYPE)

OBJS = $(addprefix $(OUTDIR)/, $(SRCS:.c=.$(ICNAME).rel))

all:
	$(MAKE) PFS173
	$(PROG) -s 0x30 -n PFS173 write $(TARGET)

PFS154:  $(TARGET)
PFS172:  $(TARGET)
PFS173:  $(TARGET)

$(OUTDIR)/%.$(ICNAME).rel: %.c .FORCE
	$(SDCC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(SDCC) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OUTDIR)

.FORCE:

.PHONY: all clean flash PFS173 PFS172 PFS154
