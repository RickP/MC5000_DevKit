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

all: $(TARGET)
	$(PROG) -s 31 -n $(ICNAME) write $(TARGET)

$(OUTDIR)/%.$(ICNAME).rel: %.c .FORCE
	$(SDCC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(SDCC) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OUTDIR)

.FORCE:

.PHONY: all clean flash
