SDCC        ?= sdcc
PROG        ?= easypdkprog

#setup target PFS173
ICTYPE      = pdk15
ICNAME      = PFS173

OUTDIR=../build
$(shell mkdir -p $(OUTDIR))

TARGET=$(OUTDIR)/display.$(ICNAME).ihx
SRCS := display.c

CFLAGS =-m$(ICTYPE) -D$(ICNAME) -I ../lib 
LDFLAGS=-m$(ICTYPE)

OBJS = $(addprefix $(OUTDIR)/, $(SRCS:.c=.$(ICNAME).rel))

all: $(TARGET)

flash: $(TARGET)
	$(PROG) -n $(ICNAME) write $(TARGET)

$(OUTDIR)/%.$(ICNAME).rel: %.c .FORCE
	$(SDCC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(SDCC) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OUTDIR)

.FORCE:

.PHONY: all clean flash
