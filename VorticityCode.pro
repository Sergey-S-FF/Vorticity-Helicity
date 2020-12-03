QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp
INCLUDEPATH += /home/sergey/mpdroot/bbc \
               /home/sergey/mpdroot/bmd \
               /home/sergey/mpdroot/cpc \
               /home/sergey/mpdroot/clustering \
               /home/sergey/mpdroot/dch \
               /home/sergey/mpdroot/emc \
               /home/sergey/mpdroot/etof \
               /home/sergey/mpdroot/eventdisplay \
               /home/sergey/mpdroot/ffd \
               /home/sergey/mpdroot/fsa \
               /home/sergey/mpdroot/generators \
               /home/sergey/mpdroot/kalman \
               /home/sergey/mpdroot/lhetrack \
               /home/sergey/mpdroot/mcstack \
               /home/sergey/mpdroot/mpdbase \
               /home/sergey/mpdroot/mpddst \
               /home/sergey/mpdroot/mpdfield \
               /home/sergey/mpdroot/mpdpid \
               /home/sergey/mpdroot/passive \
               /home/sergey/mpdroot/physics \
               /home/sergey/mpdroot/physics/femto \
               /home/sergey/mpdroot/sft \
               /home/sergey/mpdroot/shield_pack \
               /home/sergey/mpdroot/strawECT \
               /home/sergey/mpdroot/strawendcap \
               /home/sergey/mpdroot/stt \
               /home/sergey/mpdroot/sts \
               /home/sergey/mpdroot/tof \
               /home/sergey/mpdroot/tpc \
               /home/sergey/mpdroot/zdc \
               /home/sergey/mpdroot/tgem \
               /opt/fairsoft/install/include/root6 \
               /opt/fairroot/install/include \
               /opt/fairsoft/install/include \
               #/home/ubuntu/root-6/include \

LIBS += -L/home/sergey/mpdroot/build/lib  -lMpdBase -lKalman \
            -lEmc -lEtof -lEventDisplay -lFfd -lHADGEN \
            -lLHETrack  -lMpdDst -lMpdField \
            -lMpdGeneralGenerator \
            -lMpdGen -lMpdPid -lPassive -lStrawECT \
            -lbmd -lCluster -lCpc \
            -lStrawendcap -lSts -lTHadgen \
            -lTof -ltpc -lTShieldGenerator \
            -lTShield -lZdc \
            -lMpdMCStack -lMpdFemtoMaker \  #-lMpdFemto \
        -L/opt/fairroot/build/lib -lBase -lEventDisplay -lFairDataMatch -lFairTools -lMCStack \
                -lGeane -lGen -lGeoBase -lMbsAPI -lParBase -lTrkBase \
                -lParMQ -lBaseMQ -lMbsAPI -lAlignment -lFairFastSim -lMCConfigurator -lFairMCStepLogger \
        -L/opt/fairsoft/install/lib/root -lGui -lCore -lImt -lRIO -lNet -lHist \
            -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint \
            -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc \
            -lROOTDataFrame -pthread -Wl,-rpath,/opt/fairsoft/install/lib/root -lm -ldl -rdynamic
        #-L/home/ubuntu/root-6/lib -lCore \

#LIBS += -L/home/ubuntu/root-6/lib -lCore
