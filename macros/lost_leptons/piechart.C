// plot_distribution: Macro that plots variables both lumi weighted and normalized to the same area.

#define INT_ROOT
// #include "inc/mfs_style.hpp"
// #include "src/mfs_style.cpp"
// #include "inc/mfs_utilities.hpp"
// #include "src/mfs_utilities.cpp"

#include <iostream>
#include <vector>
#include "TPie.h"
#include "TPieSlice.h"
#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TCut.h"
#include "macros/include/my_style.hpp"

using namespace std;
using std::cout;
using std::endl;

// class sfeats {
// public:
//   sfeats(TString icut, TString iname, int icolor){
//     cut = icut; name = iname; color = icolor;
//   }
//   TString cut, name;
//   int color;
// };

void piechart(TString sample, TCut more_cuts="", TString tag="ttbar_baseline") { 
  // styles style("Standard"); style.setDefaultStyle();
  gStyle->SetTextSize(0.05);            // Set global text size
  gStyle->SetTitleSize(0.05);     // Set the 2 axes title size
  gStyle->SetTitleFont(132);     
  gStyle->SetLabelSize(0.05);     // Set the 2 axes label size
  gStyle->SetLabelFont(132);    
  gStyle->SetCanvasDefW(1400);

  TCanvas can;
  TString pname, Hname = "histo", title;

  TChain* chain= new TChain("reduced_tree");
 
  chain->Add(sample);

  TH1D histo(Hname, "",100, 0, 10000);

  chain->Project(Hname, "met", "(weightppb*4000)");
  cout << "Chain contains " << chain->GetEntries() << " entries." << endl;
  cout << "Chain contains " << histo.Integral(1,101) << " weighted events." << endl;

  TCut ht("ht30>500"), njets("jet4_pt>30"), mht30("mht30>200"), mdp("min_delta_phi_met_N>4"), nb("num_csvm_jets30>=2"), presel(ht+njets+mht30+mdp);

  TCut mu_veto("num_reco_veto_muons==0"), el_veto("num_reco_veto_electrons==0");
  TCut cuts(presel+mu_veto+el_veto);
    // TCut cuts("mht30>200&&ht30>500&&min_delta_phi_met_N>4&&num_jets_pt30>=4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>=2");
  // TCut cuts("mht30>200&&ht30>500&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_jets_pt30>=4");
  cuts+=more_cuts;
  vector<TCut> comp;
  comp.push_back("(0x0FFF&type_code)==0");//,"Hadronic",1));
  comp.push_back("(0x0FFF&type_code)==0x0100");//,"1 lep",4));
  comp.push_back("(0x0FFF&type_code)==0x0001");//,"1 #tau_{had}",28));
  comp.push_back("(0x0FFF&type_code)==0x0111");//,"1 #tau_{lep}",kYellow));
  comp.push_back("((0x000F&type_code)>=2) || (((0x0F00&type_code)/0x0100)>=2) || ((0x0FFF&type_code)==0x0101)");//,">=2 e/mu/tau",2));
  
  vector<TString> labels;
  labels.push_back("Hadronic");
  labels.push_back("1 W #rightarrow e/#mu");
  labels.push_back("1 W #rightarrow #tau #rightarrow had");
  labels.push_back("1 W #rightarrow #tau #rightarrow e/#mu");
  labels.push_back("#geq 2 W #rightarrow e/#mu/#tau");
  vector<float> yields;
  //vector<string> labels;
  vector<int> colors;
  colors.push_back(1);
  colors.push_back(4);
  colors.push_back(kGreen+2);
  colors.push_back(kYellow);
  colors.push_back(2);
  // colors.push_back(kCyan+2);


  for(unsigned ind(0); ind<comp.size(); ind++){
    // totCut = "weightppb*4000*("+cuts+"&&"+comp[ind].Data()+")";
    TCut totCut = (cuts+comp[ind])*"(weightppb*4000)";
    //    cout << totCut.GetTitle() << endl;
    // if (sample.Contains("TTJets")) totCut=(cuts+comp[ind])*"(3.17760399999999981e-05*4000)";
    chain->Project(Hname, "met", totCut);
    yields.push_back(histo.Integral(0,101));
  }

  TPie pie("pie1", title, yields.size(), &yields[0]);
  pie.SetFillColors(&colors[0]);
  // pie.SetLabels(&labels[0]);
  for(unsigned ind(0); ind<comp.size(); ind++){
    pie.GetSlice(ind)->SetTitle(labels[ind].Data());
    //  pie.GetSlice(ind)->SetTitleFont(132);
    //  pie->GetSlice(ind)->SetTitleColor(colors[ind]);
  }
  // pie.SetLabelFormat("%txt: %val (%perc)");
  pie.SetLabelFormat("#splitline{%txt}{%val (%perc)}");
  pie.SetLabelsOffset(0.03);
  pie.SetRadius(0.3);
  // pie.Draw("3d"); 
  pie.Draw("");
  //  pname = "macros/lost_leptons/output/lepton_piechart_"+tag+".root";
  // pie.SaveAs(pname);
  pname = "macros/lost_leptons/plots/lepton_piechart_"+tag+".pdf";
  can.SaveAs(pname);
  


}
