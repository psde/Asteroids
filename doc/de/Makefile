NAME = doc_de
SRC_DIR = src/
MAINTEX = $(SRC_DIR)/$(NAME).tex
SRC_FILES = $(wildcard $(SRC_DIR)/*.tex)
BIB_FILE = $(SRC_DIR)/$(NAME)

OUTPUT_DIR = out/
PDFLATEX = pdflatex -shell-escape -output-directory $(OUTPUT_DIR)
BIBER = biber --output_directory=$(OUTPUT_DIR)
CP_CMD = cp $(OUTPUT_DIR)$(NAME).pdf .

.PHONY: all fast full clean   

all: fast

fast: $(SRC_FILES)
	  $(PDFLATEX) $(MAINTEX)
	  $(CP_CMD)

full: $(SRC_FILES) clean
	  $(PDFLATEX) $(MAINTEX)
	  $(PDFLATEX) $(MAINTEX)
	  #$(BIBER) $(BIB_FILE)
	  $(PDFLATEX) $(MAINTEX)
	  #$(PDFLATEX) $(MAINTEX)
	  $(CP_CMD)
	  
RMCMD = rm -f $(NAME).pdf *.lof *.lol *.bcf *.tdo *.run.xml *.brf *.acn *.acr *.alg *.glg *.glo *.gls *.idx *.ilg *.ind *.ist *.lot *.aux *.bbl *.blg *.brf *.out *.log *.toc *.ppt *.tpt *.synctex.gz
clean:
	  cd out; $(RMCMD); cd src; $(RMCMD)