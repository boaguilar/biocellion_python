TARGETS := command-line-tools.pdf

all: $(TARGETS)

view: $(TARGETS)
	evince $(TARGETS)

clean:
	rm -f *.aux *.log *.dvi *~

spotless:
	make clean
	rm -f *.ps *.pdf

%.pdf: %.tex
	pdflatex $*.tex
	pdflatex $*.tex
