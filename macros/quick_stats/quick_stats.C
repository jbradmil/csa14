#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"

void quick_stats(TString files) {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  int nTotal(chain->GetEntries()), n_true_mu(chain->GetEntries("num_gen_muons==1")), n_lost_mu(chain->GetEntries("num_gen_muons==1&&muon_veto<1"));
  // n_lost_mu = chain->GetEntries("num_gen_muons==1&&num_reco_veto_muons==0");
  TCut matched("num_gen_muons==1&&muon_reco_match>=0");
  int n_no_match(chain->GetEntries("num_gen_muons==1"+!matched)), n_fail_iso(chain->GetEntries(matched+"muon_relIso>0.2&&muon_pt>10")), n_fail_pt(chain->GetEntries(matched+"muon_pt<10&&muon_relIso<0.2"));
  int n_fail_both(chain->GetEntries(matched+"muon_relIso>0.2&&muon_pt<10")), n_fail_either(chain->GetEntries(matched+"muon_relIso>0.2||muon_pt<10"));

  printf("Sample: %s\n", files.Data());
  printf("Events: %d\n", nTotal);
  printf("1 true mu: %d (%3.1f %%)\n", n_true_mu, 100*n_true_mu/(float)nTotal);
  printf("1 lost mu: %d (%3.1f %%)\n\n", n_lost_mu, 100*n_lost_mu/(float)n_true_mu);

  printf("Out-of-acceptance: %d (%3.1f %%)\n", n_no_match, 100*n_no_match/(float)n_lost_mu);
  printf("Fails pt: %d (%3.1f %%)\n", n_fail_pt, 100*n_fail_pt/(float)n_lost_mu);
  printf("Fails iso: %d (%3.1f %%)\n", n_fail_iso, 100*n_fail_iso/(float)n_lost_mu);
  printf("Fails both: %d (%3.1f %%)\n", n_fail_both, 100*n_fail_both/(float)n_lost_mu);
  printf("Fails either: %d (%3.1f %%)\n", n_fail_either, 100*n_fail_either/(float)n_lost_mu);
}
