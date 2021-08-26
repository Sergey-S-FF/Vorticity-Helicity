CONFIG += c++11 console

SOURCES += main.cpp

INCLUDEPATH += /home/ubuntu/rootcern/mpdroot/bbc \
               /home/ubuntu/rootcern/mpdroot/bmd \
               /home/ubuntu/rootcern/mpdroot/cpc \
               /home/ubuntu/rootcern/mpdroot/clustering \
               /home/ubuntu/rootcern/mpdroot/dch \
               /home/ubuntu/rootcern/mpdroot/emc \
               /home/ubuntu/rootcern/mpdroot/etof \
               /home/ubuntu/rootcern/mpdroot/eventdisplay \
               /home/ubuntu/rootcern/mpdroot/ffd \
               /home/ubuntu/rootcern/mpdroot/fsa \
               /home/ubuntu/rootcern/mpdroot/generators \
               /home/ubuntu/rootcern/mpdroot/kalman \
               /home/ubuntu/rootcern/mpdroot/lhetrack \
               /home/ubuntu/rootcern/mpdroot/mcstack \
               /home/ubuntu/rootcern/mpdroot/mpdbase \
               /home/ubuntu/rootcern/mpdroot/mpddst \
               /home/ubuntu/rootcern/mpdroot/mpdfield \
               /home/ubuntu/rootcern/mpdroot/mpdpid \
               /home/ubuntu/rootcern/mpdroot/passive \
               /home/ubuntu/rootcern/mpdroot/physics \
               /home/ubuntu/rootcern/mpdroot/physics/femto \
               /home/ubuntu/rootcern/mpdroot/sft \
               /home/ubuntu/rootcern/mpdroot/shield_pack \
               /home/ubuntu/rootcern/mpdroot/strawECT \
               /home/ubuntu/rootcern/mpdroot/strawendcap \
               /home/ubuntu/rootcern/mpdroot/stt \
               /home/ubuntu/rootcern/mpdroot/sts \
               /home/ubuntu/rootcern/mpdroot/tof \
               /home/ubuntu/rootcern/mpdroot/tpc \
               /home/ubuntu/rootcern/mpdroot/zdc \
               /home/ubuntu/rootcern/mpdroot/tgem \
               /home/ubuntu/rootcern/mpdroot/macro \
               /home/ubuntu/rootcern/fairsoft/install/include/root6 \
               /home/ubuntu/rootcern/fairroot/install/include \
               /home/ubuntu/rootcern/fairsoft/install/include \
               #/home/ubuntu/root-6/include \

LIBS += -L/home/ubuntu/rootcern/mpdroot/build/lib  -lMpdBase -lKalman \
            -lEmc -lEtof -lEventDisplay -lFfd -lHADGEN \
            -lLHETrack  -lMpdDst -lMpdField \
            -lMpdGeneralGenerator \
            -lMpdGen -lMpdPid -lPassive \# -lStrawECT \
            -lbmd -lCluster -lCpc \            
           # -lStrawendcap \
            -lSts -lTHadgen \
            -lTof -ltpc -lTShieldGenerator \
            -lTShield -lZdc \
            -lMpdMCStack -lMpdFemtoMaker \  #-lMpdFemto \
        -L/home/ubuntu/rootcern/fairroot/build/lib -lBase -lEventDisplay -lFairDataMatch -lFairTools \ # -lMCStack \
                -lGeane -lGen -lGeoBase -lMbsAPI -lParBase -lTrkBase \
                -lParMQ -lBaseMQ -lMbsAPI -lAlignment -lFairFastSim -lMCConfigurator -lFairMCStepLogger \
        -L/home/ubuntu/rootcern/fairsoft/install/lib/root -lGui -lCore -lImt -lRIO -lNet -lHist \
            -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint \
            -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc \
            -lROOTDataFrame -lMathMore -pthread -Wl,-rpath,/home/ubuntu/rootcern/fairsoft/install/lib/root -lm -ldl -rdynamic
        #-L/home/ubuntu/root-6/lib -lCore \

#LIBS += -L/home/ubuntu/root-6/lib -lCore
