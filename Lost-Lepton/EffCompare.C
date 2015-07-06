#include "TFile.h"
#include "TString.h"
#include <iostream>
#include <TH2.h>
#include <vector>
#include "TCanvas.h"
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>

class TH1FCompare
{
public:
	TH1FCompare(){}
	TH1FCompare(TString name1, TString name2, TH1F *th1f1, TH1F *th1f2, TFile *outPutFile, TString TH1Name)
	{
		name1_=name1;
		name2_=name2;
		th1f1_=th1f1;
		th1f2_=th1f2;
		outPutFile_=outPutFile;
		TH1Name_=TH1Name;
		Compare();
	}
	~TH1FCompare(){}
private:
	TString name1_, name2_, TH1Name_;
	TH1F *th1f1_, *th1f2_;
	TFile *outPutFile_;
	TCanvas *Both_;
	void Compare();
};
	
void TH1FCompare::Compare()
{
	gStyle->SetPaintTextFormat("5.2f");
	gStyle->SetStatW(0.1);
	gStyle->SetStatH(0.1);
	gStyle->SetStatY(202);
	gStyle->SetTitleYOffset(1.3);
 	gROOT->SetBatch(true);
 	Both_ = new TCanvas(TH1Name_,TH1Name_,600,600);
	Both_->cd();
// 	Both_->Divide(1,2,0.01,0.01);
	TPad *mainPad = new TPad("pad","",0,0.185,1,1);
	mainPad->SetFillStyle(0);
	mainPad->SetFrameFillColor(10);
	mainPad->SetFrameBorderMode(0);
	mainPad->Draw();
	mainPad->cd();
	TLegend *lLeg = new TLegend(0.45,0.7,0.90,0.85);
	lLeg->SetFillColor(0);
	mainPad->SetLogx();
	th1f1_->SetMarkerColor(1);
	th1f1_->SetMarkerSize(4);
	th1f1_->SetLineColor(1);
	th1f1_->SetLineWidth(2);
	th1f1_->SetMarkerSize(4.0);
// 	th1f1_->UseCurrentStyle();
	th1f1_->Draw("E");
	lLeg->AddEntry(th1f1_,name1_,"l");
	th1f2_->SetMarkerColor(4);
	th1f2_->SetMarkerSize(4);
	th1f2_->SetLineColor(4);
	th1f2_->SetLineWidth(2);
	th1f2_->SetMarkerSize(4.0);
// 	th1f2_->UseCurrentStyle();
	th1f2_->Draw("ESame");
	lLeg->AddEntry(th1f2_,name2_,"l");
	lLeg->Draw();
	// ratio
	Both_->cd();
	TPad *ratioPad = new TPad("bpad","",0,0,1,0.3);
	ratioPad->SetFillStyle(0);
	ratioPad->SetFrameFillColor(10);
	ratioPad->SetFrameBorderMode(0);
	ratioPad->Draw();
	ratioPad->cd();
	ratioPad->SetLogx();
	TH1F *ratio = (TH1F*) th1f1_->Clone();
	ratio->Divide(th1f2_);
	ratio->SetTitle("");
	ratio->SetYTitle("ratio");
	ratio->Draw("E");
	Both_->Update();
	outPutFile_->cd();
	Both_->Write();
	TString temp = TH1Name_ + "_ratio";
	Both_->SaveAs(temp+".pdf");
 	delete Both_;
 	gROOT->SetBatch(false);
}


class TH2FCompare
{
public:
	TH2FCompare(){}
	TH2FCompare(TString name1, TString name2, TH2F *th2f1, TH2F *th2f2, TFile *outPutFile, TString TH1Name)
	{
		name1_=name1;
		name2_=name2;
		th2f1_=th2f1;
		th2f2_=th2f2;
		outPutFile_=outPutFile;
		TH1Name_=TH1Name;
		Compare();
	}
	~TH2FCompare(){}
private:
	TString name1_, name2_, TH1Name_;
	TH2F *th2f1_, *th2f2_;
	TFile *outPutFile_;
	TCanvas *Both_;
	void Compare();
};


void TH2FCompare::Compare()
{
	gStyle->SetPaintTextFormat("5.2f");
	gStyle->SetStatW(0.1);
	gStyle->SetStatH(0.1);
	gStyle->SetStatY(202);
	gStyle->SetTitleYOffset(1.3);
	gROOT->SetBatch(true);
	Both_ = new TCanvas(TH1Name_,TH1Name_,600,600);
	Both_->cd();
	Both_->SetLogx();
	Both_->SetLogy();
	th2f1_->Divide(th2f1_,th2f2_);
	TString temp = th2f1_->GetTitle();
	temp+=" , R: "+ name1_ + " / "+name2_;
	th2f1_->SetTitle(temp);
	th2f1_->Draw("ColZ,Text,E");
	Both_->Write();
	temp = TH1Name_ + "_ratio";
	Both_->SaveAs(temp+".pdf");
	 	delete Both_;
	gROOT->SetBatch(false);
}

void EffCompare()
{
	TString outPutFile = "CompareEff.root";
	TFile *out_ = new TFile(outPutFile,"RECREATE");
	
	TString file1 = "Efficiencies.root";
	TString FolderFile1 = "Efficiencies";
	TString name1 = "t#bar{t}, W#rightarrowl#nu+jets (truth)";
	
	TString file2 = "Efficiencies2.root";
	TString FolderFile2 = "Efficiencies";
	TString name2 = "DY (truth)";
	
	std::vector<TString> TH1FNames_, TH2FNames_;
	// list of TH1 and TH2 to compare
	TH1FNames_.push_back("MuRecoActivity");
	
	TH2FNames_.push_back("MuRecoPTActivity");
	TH2FNames_.push_back("MuIsoPTActivity");
	TH2FNames_.push_back("ElecRecoPTActivity");
	TH2FNames_.push_back("ElecIsoPTActivity");
	
	std::cout<<"Comparing file: "<<file1<<" with: "<<file2<<std::endl;
	TFile *f1, *f2;
	f1 = new TFile(file1,"READ");
	f2 = new TFile(file2,"READ");
	std::cout<<"Files opend. \n";
	TDirectory *Folder1 =   (TDirectory*)f1->Get(FolderFile1);
	TDirectory *Folder2 =   (TDirectory*)f2->Get(FolderFile2);
	std::cout<<"Folder loaded. \n";
	for(unsigned int i=0; i< TH1FNames_.size();i++)
	{
		TH1FCompare(name1, name2,(TH1F*) Folder1->Get(TH1FNames_[i]), (TH1F*) Folder2->Get(TH1FNames_[i]), out_, TH1FNames_[i]);
		
	}
	for(unsigned int i=0; i< TH2FNames_.size();i++)
	{
		TH2FCompare(name1, name2,(TH2F*) Folder1->Get(TH2FNames_[i]), (TH2F*) Folder2->Get(TH2FNames_[i]), out_, TH2FNames_[i]);
	}
	
	out_->Close();
	std::cout<<"\nDone.\n";
	delete f1;
	delete f2;
	
}
