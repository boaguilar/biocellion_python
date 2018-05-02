TARGET := models.pdf
MAIN_FILE := models.tex
MAIN_AUX := models.aux
DEP_FILES := \
	models.tex \
	xml.tex \
	model.tex \
	model.simulator.tex \
	model.simulator.time_step.tex \
	model.reaction.tex \
	model.reaction.yield.tex \
	model.reaction.kineticFactor.tex \
	model.solver.tex \
	param.tex

all: $(TARGET)

view: $(TARGET)
	evince $(TARGET)

$(TARGET): $(DEP_FILES)
	pdflatex $(MAIN_FILE)
	#bibtex $(MAIN_AUX)
	pdflatex $(MAIN_FILE)
	pdflatex $(MAIN_FILE)

clean:
	rm -f *.aux *.log *.dvi *~
	-rm -f *.bbl *.blg
