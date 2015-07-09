FILE* ofile;

void PrintFileHeader() {
  fprintf(ofile, "\\documentclass[11pt, oneside]{article}\n");
  fprintf(ofile, "\\usepackage{geometry}\n");                	
  fprintf(ofile, "\\geometry{letterpaper}\n");           
  fprintf(ofile, "\\usepackage{graphicx}\n");
  fprintf(ofile, "\\usepackage{caption}\n");
  fprintf(ofile, "\\usepackage{subcaption}\n");		
  fprintf(ofile, "\\usepackage{amssymb}\n");
  fprintf(ofile, "\\usepackage{datetime}\n");
  fprintf(ofile, "\\usepackage{color}\n");
  fprintf(ofile, "\n");
  fprintf(ofile, "\\title{Lost-lepton extrapolation studies}\n");
  fprintf(ofile, " \\author{Jack Bradmiller-Feld}\n");
  fprintf(ofile, " \\date{\\today}\n");
  fprintf(ofile, "\n\n\n");
  fprintf(ofile, "\\begin{document}\n");
  fprintf(ofile, "\\maketitle\n");
}

void PrintFileTrailer() {
 
  fprintf(ofile, "\n\n\n");
  fprintf(ofile, "\\section{More closure tests}\n\n");
  fprintf(ofile, "In this section, the $\\Delta\\theta_{T}$ histograms are \\textcolor{red}{red} and the true MC historgrams are \\textcolor{blue}{blue}.\n\n");

  fprintf(ofile, "\\begin{figure}[h]\n");
  fprintf(ofile, "\\centering\n");
  fprintf(ofile, "\\caption{High-to-low MHT ratios, in bins of other search variables}\n");
  fprintf(ofile, "\\includegraphics[width=\\textwidth]{outputfiles/plot-dtt-analysis3-mhtratios.pdf}\n");
  fprintf(ofile,"\\end{figure}\n\n\n");

  fprintf(ofile, "\\begin{figure}[h]\n");
  fprintf(ofile, "\\centering\n");
  fprintf(ofile, "\\caption{High-to-low MHT ratios used in this analysis, in merged superbins}\n");
  fprintf(ofile, "\\includegraphics[width=\\textwidth]{outputfiles/plot-dtt-analysis3-superbins-mhtratios1.pdf}\n");
  fprintf(ofile,"\\end{figure}\n\n\n");

  fprintf(ofile, "\\begin{figure}[h]\n");
  fprintf(ofile, "\\centering\n");
  fprintf(ofile, "\\caption{Estimated and true event yields in analysis superbins}\n");
  fprintf(ofile, "\\includegraphics[width=\\textwidth]{outputfiles/plot-dtt-analysis3-asuperbins-bg-events.pdf}\n");
  fprintf(ofile,"\\end{figure}\n\n\n");

  fprintf(ofile, "\\clearpage");

  fprintf(ofile, "\\begin{figure}[h]\n");
  fprintf(ofile, "\\centering\n");
  fprintf(ofile,  "\\caption{MHT shapes, binned in other search variables}\n");
  fprintf(ofile,  "\\begin{subfigure}[b]{0.3\\textwidth}\n");
  fprintf(ofile,    "\\includegraphics[width=\\textwidth]{outputfiles/plot-dtt-analysis3-mht-ht.pdf}\n");
  fprintf(ofile,    "\\caption{Bins of $H_{T}$}\n");
  fprintf(ofile,  "\\end{subfigure}\n");
  fprintf(ofile,  "\\begin{subfigure}[b]{0.3\\textwidth}\n");
  fprintf(ofile,    "\\includegraphics[width=\\textwidth]{outputfiles/plot-dtt-analysis3-mht-njet.pdf}\n");
  fprintf(ofile,    "\\caption{Bins of $N_{j}$}\n");
  fprintf(ofile,  "\\end{subfigure}\n");
  fprintf(ofile,  "\\begin{subfigure}[b]{0.33\\textwidth}\n");
  fprintf(ofile,    "\\includegraphics[width=\\textwidth]{outputfiles/plot-dtt-analysis3-mht-nb.pdf}\n");
  fprintf(ofile,    "\\caption{Bins of $N_{b}$}\n");
  fprintf(ofile,  "\\end{subfigure}\n");
  fprintf(ofile, "\\end{figure}\n\n\n");
  
  fprintf(ofile, "\\end{document}");
}
