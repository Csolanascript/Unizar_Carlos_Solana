SRCS:=$(wildcard *.java)

JAVAC:=javac

CLSS:=$(SRCS:.java=.class)

all: $(CLSS)

%.class: %.java
	$(JAVAC) $<

clean:
	@rm -f $(CLSS) core tags

