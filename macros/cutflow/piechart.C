// plot_distribution: Macro that plots variables both lumi weighted and normalized to the same area.

#define INT_ROOT
#include "inc/mfs_style.hpp"
#include "src/mfs_style.cpp"
#include "inc/mfs_utilities.hpp"
#include "src/mfs_utilities.cpp"

#include "TPie.h"
#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"

using namespace std;
using std::cout;
using std::endl;

class sfeats {
public:
  sfeats(TString icut, TString iname, int icolor){
    cut = icut; name = iname; color = icolor;
  }
  TString cut, name;
  int color;
};

void piechart() { 
  // styles style("Standard"); style.setDefaultStyle();
  gStyle->SetTextSize(0.05);            // Set global text size
  gStyle->SetTitleSize(0.05);     // Set the 2 axes title size
  gStyle->SetLabelSize(0.05);     // Set the 2 axes label size
  gStyle->SetCanvasDefW(1400);

  TCanvas can;
  TString pname, Hname = "histo", totCut, tag, title;
  TH1D histo(Hname, "",100, 0, 10000);


  TChain chain("reduced_tree");
  chain.Add("reduced_trees/skimmed/TTJets*v76*");

  TString cuts("met>400&&ht30>1000&&num_csvm_jets>1&&min_delta_phi_met_N>4&&num_jets_pt30>=10&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  vector<TString> comp;
  comp.push_back("(0x0FFF&type_code)==0");//,"Hadronic",1));
  comp.push_back("(0x0FFF&type_code)==0x0100");//,"1 lep",4));
  comp.push_back("(0x0FFF&type_code)==0x0001");//,"1 #tau_{had}",28));
  comp.push_back("(0x0FFF&type_code)==0x0111");//,"1 #tau_{lep}",kYellow));
  comp.push_back("(0x0FFF&type_code)>0x0100&&(0x0FFF&type_code)<0x1000");//,">=2 lep",2));
  comp.push_back("(0x0FFF&type_code)>0x0001&&(0x0FFF&type_code)<0x0010");//,">=2 #tau",kCyan+2));

  // const_char* labels[100];
  // labels[0]=("Hadronic");
  // labels[1]=("1 W #rightarrow e/#mu");
  // labels[2]=("1 W #rightarrow #tau #rightarrow had");
  // labels[3]=("1 W #rightarrow #tau #rightarrow e/#mu");
  // labels[4]=("2 W #rightarrow e/#mu");
  // labels[5]=("2 W #rightarrow #tau");

    vector<float> yields;
    //vector<string> labels;
    vector<int> colors;
    colors.push_back(1);
    colors.push_back(4);
    colors.push_back(28);
    colors.push_back(kYellow);
    colors.push_back(2);
    colors.push_back(kCyan+2);


    for(unsigned ind(0); ind<comp.size(); ind++){
      totCut = "weightppb*5000*("+cuts+"&&"+comp[ind].Data()+")";
      chain.Project(Hname, "met", totCut);
      yields.push_back(histo.Integral(0,101));
    }

    TPie pie("pie1", title, yields.size(), &yields[0]);
    pie.SetFillColors(&colors[0]);
    // pie.SetLabels(labels[0]);
    pie.SetLabelFormat("%perc");
    pie.SetRadius(0.3);
    pie.Draw("3d");
    pname = "macros/cutflow/output/ttpiechart_"+tag+".root";
    pie.SaveAs(pname);
    pname = "macros/cutflow/plots/ttpiechart_"+tag+".pdf";
    can.SaveAs(pname);
  }


}
