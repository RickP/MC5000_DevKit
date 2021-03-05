SDCC        ?= sdcc
PROG        ?= easypdkprog

#setup target PFS173
ICTYPE      = pdk15
ICNAME      = PFS173

OUTDIR=../build
$(shell mkdir -p $(OUTDIR))

TARGET=$(OUTDIR)/MC5000.$(ICNAME).ihx
SRCS := MC5000.c

CFLAGS =-m$(ICTYPE) -D$(ICNAME) -I ../lib 
LDFLAGS=-m$(ICTYPE)

OBJS = $(addprefix $(OUTDIR)/, $(SRCS:.c=.$(ICNAME).rel))

all: $(TARGET)

flash1: $(TARGET)
	$(PROG) -s 31 -n $(ICNAME) write $(TARGET)
    
flash2: $(TARGET)
	$(PROG) -s 32 -n $(ICNAME) write $(TARGET)

$(OUTDIR)/%.$(ICNAME).rel: %.c .FORCE
	$(SDCC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(SDCC) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OUTDIR)

.FORCE:

.PHONY: all clean flash1 flash2
