#!/bin/bash

COMPR=reduced\_trees\/13TeV\/SMS\-T1tttt\_2J\_mGl\-1200\_mLSP\-800\_Tune4C\_13TeV\-madgraph\-tauola\_Phys14DR\-PU20bx25\_tsg\_PHYS14\_25\_V1\-v1\_MINIAODSIM\_UCSB2301\_v77\/\*root
NONCOMPR=reduced\_trees\/13TeV\/SMS\-T1tttt\_2J\_mGl\-1500\_mLSP\-100\_Tune4C\_13TeV\-madgraph\-tauola\_Phys14DR\-PU20bx25\_tsg\_PHYS14\_25\_V1\-v1\_MINIAODSIM\_UCSB2299\_v77\/\*root

AND=\&\&
OR=\|\|
NOT=\!

HIGHMHT=mht30\>500
HIGHHT=ht30\>1000

LOWNB=num\_csvm\_jets30\<2
HIGHNB=num\_csvm\_jets30\>\=2
NB0=num\_csvm\_jets30\=\=0
NB1=num\_csvm\_jets30\=\=1
NB2=num\_csvm\_jets30\=\=2
NBG2=num\_csvm\_jets30\>\=2
NB3=num\_csvm\_jets30\>\=3

NJ1=num\_jets\_pt30\>\=4\&\&num\_jets\_pt30\<\=6
NJ2=num\_jets\_pt30\>\=7\&\&num\_jets\_pt30\<\=8
NJ3=num\_jets\_pt30\>\=9

MUVETO=num\_veto\_mus\=\=0
ELVETO=num\_veto\_els\=\=0
LEPVETO=$MUVETO$AND$ELVETO

MINIMUVETO=Sum\$\(mu\_pt\>10\&\&mu\_mini\_iso\_tr02\<0.4\)\=\=0
MINIELVETO=Sum\$\(el\_pt\>10\&\&el\_mini\_iso\_tr02\<0.1\)\=\=0
MINILEPVETO=$MINIMUVETO$AND$MINIELVETO

TAUVETO=num\_veto\_taus\_mT\=\=0
TKVETO=num\_iso\_tracks\_pt15\_mT\=\=0

MUTKVETO=num\_mu\_tracks\_mT\=\=0
ELTKVETO=num\_el\_tracks\_mT\=\=0
HADTKVETO=num\_had\_tracks\_mT\=\=0
TK3VETO=num\_el\_tracks\_mT\+num\_mu\_tracks\_mT\+num\_had\_tracks\_mT\=\=0

MUTKVETO2=Sum\$\(mu\_track\_pt\>5\&\&mu\_track\_mT\<100\&\&\(\(mu\_track\_pt\>20\&\&mu\_track\_ch\_iso\<0.2\)\|\|\(mu\_track\_pt\>5\&\&mu\_track\_pt\<20\&\&mu\_track\_ch\_iso*mu\_track\_pt\<10\)\)\)==0
ELTKVETO2=Sum\$\(el\_track\_pt\>5\&\&el\_track\_mT\<100\&\&\(\(el\_track\_pt\>20\&\&el\_track\_ch\_iso\<0.2\)\|\|\(el\_track\_pt\>5\&\&el\_track\_pt\<20\&\&el\_track\_ch\_iso*el\_track\_pt\<5\)\)\)==0
HADTKVETO2=Sum\$\(had\_track\_pt\>10\&\&had\_track\_mT\<100\&\&\(\(had\_track\_pt\>20\&\&had\_track\_ch\_iso\<0.1\)\|\|\(had\_track\_pt\>10\&\&had\_track\_pt\<20\&\&had\_track\_ch\_iso*had\_track\_pt\<3\)\)\)==0
TK4VETO=$MUTKVETO2$AND$ELTKVETO2$AND$HADTKVETO2

BBBBNC=$NBG2\&\&ht30\>1200\&\&mht30\>500
BBBBC=$NBG2\&\&ht30\>500\&\&mht30\>500

TTTTNC=$NBG2\&\&ht30\>1000\&\&mht30\>500\&\&num\_jets\_pt30\>\=9
TTTTC=$NBG2\&\&ht30\>500\&\&mht30\>500

MULOWPT=mu\_pt\[0\]\>10\&\&mu\_pt[0]\<20
MUHIGHPT=mu\_pt[0]\>20

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"\",\"no\_veto\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"$LOWNB\",\"no\_veto\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"$HIGHNB\",\"no\_veto\_highnb\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"\",\"mu\_veto\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$LOWNB\",\"mu\_veto\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$HIGHNB\",\"mu\_veto\_highnb\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"\",\"el\_veto\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$LOWNB\",\"el\_veto\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$HIGHNB\",\"el\_veto\_highnb\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"\",\"lep\_veto\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$LOWNB\",\"lep\_veto\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHNB\",\"lep\_veto\_highnb\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"\",\"mu\_veto\_mini\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$LOWNB\",\"mu\_veto\_mini\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB\",\"mu\_veto\_mini\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB$AND$MULOWPT\",\"mu\_veto\_mini\_highnb_lowpt\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB$AND$MUHIGHPT\",\"mu\_veto\_mini\_highnb_highpt\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"\",\"el\_veto\_mini\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$LOWNB\",\"el\_veto\_mini\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$HIGHNB\",\"el\_veto\_mini\_highnb\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"\",\"lep\_veto\_mini\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$LOWNB\",\"lep\_veto\_mini\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHNB\",\"lep\_veto\_mini\_highnb\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHMHT\",\"lep\_veto\_highmht\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHMHT\",\"lep\_veto\_highmht\_mini\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHHT\",\"lep\_veto\_highht\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHHT\",\"lep\_veto\_highht\_mini\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHMHT$AND$HIGHNB\",\"lep\_veto\_highmht\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHMHT$AND$HIGHNB\",\"lep\_veto\_highmht\_highnb\_mini\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHHT$AND$HIGHNB\",\"lep\_veto\_highht\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHHT$AND$HIGHNB\",\"lep\_veto\_highht\_highnb\_mini\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\_mini\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"$NJ3\",\"no\_veto\_nj3\",\"\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\",\"\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\_mini\",\"\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TAUVETO\",\"$LEPVETO$AND$HIGHNB\",\"tau\_veto\_highnb\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO\",\"$LEPVETO\",\"tk\_veto\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO\",\"$LEPVETO$AND$LOWNB\",\"tk\_veto\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO\",\"$LEPVETO$AND$HIGHNB\",\"tk\_veto\_highnb\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUTKVETO\",\"$LEPVETO$AND$HIGHNB\",\"mu\_tk\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELTKVETO\",\"$LEPVETO$AND$HIGHNB\",\"el\_tk\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$HADTKVETO\",\"$LEPVETO$AND$HIGHNB\",\"had\_tk\_veto\_highnb\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO\",\"$LEPVETO\",\"tk3\_veto\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO\",\"$LEPVETO$AND$LOWNB\",\"tk3\_veto\_lownb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO\",\"$LEPVETO$AND$HIGHNB\",\"tk3\_veto\_highnb\"\);


#MUTKVETO2
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"mu\_tk2\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"el\_tk2\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$HADTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"had\_tk2\_veto\_highnb\"\);


root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK4VETO$AND$LEPVETO\",\"$HIGHNB\",\"full4\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TKVETO\",\"$HIGHNB\",\"full\_veto\_highnb\"\);


root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$HIGHNB\",\"full3\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$LOWNB\",\"full3\_veto\_lownb\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$HIGHNB\",\"full3\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$LOWNB\",\"full3\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$HIGHNB\",\"full3\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$LOWNB\",\"full3\_veto\_lownb\",\"$COMPR\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$HIGHNB\",\"full3\_mini\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$LOWNB\",\"full3\_mini\_veto\_lownb\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$HIGHNB\",\"full3\_mini\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$LOWNB\",\"full3\_mini\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$HIGHNB\",\"full3\_mini\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$LOWNB\",\"full3\_mini\_veto\_lownb\",\"$COMPR\"\);

# signal
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"\",\"no\_veto\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"$LOWNB\",\"no\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"$HIGHNB\",\"no\_veto\_highnb\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"\",\"mu\_veto\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$LOWNB\",\"mu\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$HIGHNB\",\"mu\_veto\_highnb\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"\",\"el\_veto\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$LOWNB\",\"el\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$HIGHNB\",\"el\_veto\_highnb\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"\",\"lep\_veto\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$LOWNB\",\"lep\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHNB\",\"lep\_veto\_highnb\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"\",\"mu\_veto\_mini\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$LOWNB\",\"mu\_veto\_mini\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB\",\"mu\_veto\_mini\_highnb\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"\",\"el\_veto\_mini\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$LOWNB\",\"el\_veto\_mini\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$HIGHNB\",\"el\_veto\_mini\_highnb\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"\",\"lep\_veto\_mini\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$LOWNB\",\"lep\_veto\_mini\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHNB\",\"lep\_veto\_mini\_highnb\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHMHT\",\"lep\_veto\_highmht\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHMHT\",\"lep\_veto\_highmht\_mini\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHHT\",\"lep\_veto\_highht\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHHT\",\"lep\_veto\_highht\_mini\",\"$NONCOMPR\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHMHT$AND$HIGHNB\",\"lep\_veto\_highmht\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHMHT$AND$HIGHNB\",\"lep\_veto\_highmht\_highnb\_mini\",\"$NONCOMPR\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHHT$AND$HIGHNB\",\"lep\_veto\_highht\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHHT$AND$HIGHNB\",\"lep\_veto\_highht\_highnb\_mini\",\"$NONCOMPR\"\);

# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\_mini\",\"$NONCOMPR\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"\",\"$NJ3\",\"no\_veto\_nj3\",\"\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\",\"\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$NJ3\",\"lep\_veto\_nj3\_mini\",\"\",\"$NONCOMPR\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TAUVETO\",\"$LEPVETO$AND$HIGHNB\",\"tau\_veto\_highnb\",\"$NONCOMPR\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO\",\"$LEPVETO\",\"tk\_veto\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO\",\"$LEPVETO$AND$LOWNB\",\"tk\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO\",\"$LEPVETO$AND$HIGHNB\",\"tk\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO\",\"$LEPVETO$AND$HIGHNB\",\"tk\_veto\_highnb\",\"$COMPR\"\);


root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUTKVETO\",\"$LEPVETO$AND$HIGHNB\",\"mu\_tk\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELTKVETO\",\"$LEPVETO$AND$HIGHNB\",\"el\_tk\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$HADTKVETO\",\"$LEPVETO$AND$HIGHNB\",\"had\_tk\_veto\_highnb\",\"$NONCOMPR\"\);


# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO\",\"$LEPVETO\",\"tk3\_veto\",\"$NONCOMPR\"\);
# root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO\",\"$LEPVETO$AND$LOWNB\",\"tk3\_veto\_lownb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO\",\"$LEPVETO$AND$HIGHNB\",\"tk3\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO\",\"$LEPVETO$AND$HIGHNB\",\"tk3\_veto\_highnb\",\"$COMPR\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TKVETO\",\"$HIGHNB\",\"full\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$HIGHNB\",\"full3\_veto\_highnb\",\"$NONCOMPR\"\);


root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO$AND$LEPVETO\",\"$HIGHNB\",\"full\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TKVETO$AND$LEPVETO\",\"$HIGHNB\",\"full\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO$AND$LEPVETO\",\"$HIGHNB\",\"full3\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK3VETO$AND$LEPVETO\",\"$HIGHNB\",\"full3\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK4VETO$AND$LEPVETO\",\"$HIGHNB\",\"full4\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$TK4VETO$AND$LEPVETO\",\"$HIGHNB\",\"full4\_veto\_highnb\",\"$COMPR\"\);


root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUTKVETO\",\"$HIGHNB$AND$LEPVETO\",\"mu\_tk\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELTKVETO\",\"$HIGHNB$AND$LEPVETO\",\"el\_tk\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$HADTKVETO\",\"$HIGHNB$AND$LEPVETO\",\"had\_tk\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUTKVETO\",\"$HIGHNB$AND$LEPVETO\",\"mu\_tk\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELTKVETO\",\"$HIGHNB$AND$LEPVETO\",\"el\_tk\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$HADTKVETO\",\"$HIGHNB$AND$LEPVETO\",\"had\_tk\_veto\_highnb\",\"$COMPR\"\);


root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"mu\_tk2\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"el\_tk2\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$HADTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"had\_tk2\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"mu\_tk2\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"el\_tk2\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$HADTKVETO2\",\"$HIGHNB$AND$LEPVETO\",\"had\_tk2\_veto\_highnb\",\"$COMPR\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3\",\"lep\_veto\_t1ttt\_rich\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3\",\"lep\_veto\_t1ttt\_rich\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3\",\"lep\_veto\_mini\_t1ttt\_rich\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3\",\"lep\_veto\_mini\_t1ttt\_rich\",\"$COMPR\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3$AND$HIGHMHT\",\"mu\_veto\_t1ttt\_rich\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$HIGHNB$AND$NJ3\",\"mu\_veto\_t1ttt\_rich\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3$AND$HIGHMHT\",\"mu\_veto\_mini\_t1ttt\_rich\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB$AND$NJ3\",\"mu\_veto\_mini\_t1ttt\_rich\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3$AND$HIGHMHT\",\"el\_veto\_t1ttt\_rich\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$HIGHNB$AND$NJ3\",\"el\_veto\_t1ttt\_rich\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$HIGHHT$AND$HIGHNB$AND$NJ3$AND$HIGHMHT\",\"el\_veto\_mini\_t1ttt\_rich\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$HIGHNB$AND$NJ3\",\"el\_veto\_mini\_t1ttt\_rich\",\"$COMPR\"\);

# Just mu veto
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$HIGHNB\",\"mu\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB\",\"mini\_mu\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$HIGHNB\",\"mu\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB\",\"mini\_mu\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MUVETO\",\"$HIGHNB\",\"mu\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIMUVETO\",\"$HIGHNB\",\"mini\_mu\_veto\_highnb\",\"$COMPR\"\);

# Just el veto
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$HIGHNB\",\"el\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$HIGHNB\",\"mini\_el\_veto\_highnb\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$HIGHNB\",\"el\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$HIGHNB\",\"mini\_el\_veto\_highnb\",\"$NONCOMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$ELVETO\",\"$HIGHNB\",\"el\_veto\_highnb\",\"$COMPR\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINIELVETO\",\"$HIGHNB\",\"mini\_el\_veto\_highnb\",\"$COMPR\"\);

# BBBBNC

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$BBBBNC\",\"full3\_veto\_bbbbnc\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$BBBBNC\",\"full3\_mini\_veto\_bbbbnc\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$BBBBC\",\"full3\_veto\_bbbbc\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$BBBBC\",\"full3\_mini\_veto\_bbbbc\"\);

root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$LEPVETO$AND$TK3VETO\",\"$TTTTNC\",\"full3\_veto\_ttttnc\"\);
root -b -q macros/lost_leptons/make_lost_lepton_hists.C+\(\"$MINILEPVETO$AND$TK3VETO\",\"$TTTTNC\",\"full3\_mini\_veto\_ttttnc\"\);
