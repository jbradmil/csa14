#ifndef H_REMOVE_DUPLICATE_EVENTS
#define H_REMOVE_DUPLICATE_EVENTS
// http://stackoverflow.com/questions/12002098/removing-duplicate-entries-in-a-root-ttree
#include <iostream>
#include <set>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TEntryList.h"
#include "event_number.hpp"
#include "timer.hpp"

using namespace std;

void remove_duplicate_events(TChain* tree) {

  set<int> eventIds; // keep track of already seen event numbers
  set<int> runIds;
  set<int> lumiIds;
  // set<EventNumber> eventList
  uint event, run, lumiblock;
  int nEntries = tree->GetEntries();

  tree->SetBranchAddress("event",&event);
  tree->SetBranchAddress("run",&run);
  tree->SetBranchAddress("lumiblock",&lumiblock);

  TEntryList *tlist = new TEntryList(tree); // initialize entry list for 'TTree* tree'

  // loop over the entries in 'tree'
  // Timer timer(nEntries);
  // timer.Start();   
  cout << "Looping over " << nEntries << " entries." << endl; 
  for (int j = 0; j < nEntries; ++j)
    {
      if((j%10000==0||j==1||j==10||j==100||j==1000) && j!=0) cout << "Processed " << j << endl;
      tree->GetEvent(j);

      // if we have not seen this event yet, add it to the set
      // and to the entry list
      if (eventIds.count(event) == 0 || runIds.count(run) == 0 || lumiIds.count(lumiblock) == 0)
	{
	  eventIds.insert(event);
	  runIds.insert(run);
	  lumiIds.insert(lumiblock);
	  tlist->Enter(j,tree);
	}
      //  else cout <<"Found duplicate event: " << event << " " << run << " " << lumiblock << endl;
    }

  // apply the entry list to the tree
  tree->SetEntryList(tlist);

  // delete tlist;

  // histogram of the variable 'var' will be drawn only with the

  return;

}

#endif
