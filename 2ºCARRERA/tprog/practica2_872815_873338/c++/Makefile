PROG:=main
SRCS:=main.cc instruccion.cc programa.cc add.cc dup.cc jumpif.cc mul.cc over.cc programa1.cc programa2.cc programa3.cc push.cc read.cc swap.cc write.cc

CXX:= g++ -O3

OBJS:=$(SRCS:.cc=.o)
DEPS:=$(SRCS:.cc=.d)

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.cc.o:
	$(CXX) -c -MMD $<

clean:
	@rm -f $(PROG) $(OBJS) $(DEPS) core *.s

# Dependencias
-include $(DEPS)
