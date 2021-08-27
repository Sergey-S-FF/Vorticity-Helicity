#QT += core
#QT -= gui

#CONFIG += c++11

#TARGET = test
#CONFIG += console
#CONFIG -= app_bundle

#TEMPLATE = app

#SOURCES += main.cpp




#######################
CONFIG += c++11 console

SOURCES += main.cpp

INCLUDEPATH += \
#/home/ubuntu/mpdroot/mpdroot/bbc \
#               /home/ubuntu/mpdroot/mpdroot/bmd \
#               /home/ubuntu/mpdroot/mpdroot/cpc \
#               /home/ubuntu/mpdroot/mpdroot/clustering \
#               /home/ubuntu/mpdroot/mpdroot/dch \
#               /home/ubuntu/mpdroot/mpdroot/emc \
#               /home/ubuntu/mpdroot/mpdroot/etof \
#               /home/ubuntu/mpdroot/mpdroot/eventdisplay \
#               /home/ubuntu/mpdroot/mpdroot/ffd \
#               /home/ubuntu/mpdroot/mpdroot/fsa \
#               /home/ubuntu/mpdroot/mpdroot/generators \
#               /home/ubuntu/mpdroot/mpdroot/kalman \
#               /home/ubuntu/mpdroot/mpdroot/lhetrack \
#               /home/ubuntu/mpdroot/mpdroot/mcstack \
#               /home/ubuntu/mpdroot/mpdroot/mpdbase \
#               /home/ubuntu/mpdroot/mpdroot/mpddst \
#               /home/ubuntu/mpdroot/mpdroot/mpdfield \
#               /home/ubuntu/mpdroot/mpdroot/mpdpid \
#               /home/ubuntu/mpdroot/mpdroot/passive \
#               /home/ubuntu/mpdroot/mpdroot/physics \
#               /home/ubuntu/mpdroot/mpdroot/physics/femto \
#               /home/ubuntu/mpdroot/mpdroot/sft \
#               /home/ubuntu/mpdroot/mpdroot/shield_pack \
#               /home/ubuntu/mpdroot/mpdroot/strawECT \
#               /home/ubuntu/mpdroot/mpdroot/strawendcap \
#               /home/ubuntu/mpdroot/mpdroot/stt \
#               /home/ubuntu/mpdroot/mpdroot/sts \
#               /home/ubuntu/mpdroot/mpdroot/tof \
#               /home/ubuntu/mpdroot/mpdroot/tpc \
#               /home/ubuntu/mpdroot/mpdroot/zdc \
#               /home/ubuntu/mpdroot/mpdroot/tgem \
#               /home/ubuntu/mpdroot/mpdroot/macro \
#               /home/ubuntu/mpdroot/fairsoft/install/include/root6 \
#               /home/ubuntu/mpdroot/fairroot/install/include \
#               /home/ubuntu/mpdroot/fairsoft/install/include \
/home/ubuntu/Root6.24/Root/include

#LIBS += -L/home/ubuntu/mpdroot/mpdroot/build/lib  -lMpdBase -lKalman \
#            -lEmc -lEtof -lEventDisplay -lFfd -lHADGEN \
#            -lLHETrack  -lMpdDst -lMpdField \
#            -lMpdGeneralGenerator \
#            -lMpdGen -lMpdPid -lPassive \# -lStrawECT \
#            -lbmd -lCluster -lCpc \
#           # -lStrawendcap \
#            -lSts -lTHadgen \
#            -lTof -ltpc -lTShieldGenerator \
#            -lTShield -lZdc \
#            -lMpdMCStack -lMpdFemtoMaker \ # -lMpdFemto \
#        -L/home/ubuntu/mpdroot/fairroot/build/lib -lBase -lEventDisplay -lFairDataMatch -lFairTools \ # -lMCStack \
#                -lGeane -lGen -lGeoBase -lMbsAPI -lParBase -lTrkBase \
#                -lParMQ -lBaseMQ -lMbsAPI -lAlignment -lFairFastSim -lMCConfigurator -lFairMCStepLogger \
#        -L/home/ubuntu/mpdroot/fairsoft/install/lib/root -lGui -lCore -lImt -lRIO -lNet -lHist \
#            -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint \
#            -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc \
#            -lROOTDataFrame -lMathMore -pthread -Wl,-rpath,/home/ubuntu/mpdroot/fairsoft/install/lib/root -lm -ldl -rdynamic
        #-L/home/ubuntu/root-6/lib -lCore \

LIBS += -L/home/ubuntu/Root6.24/Root/lib -lCore
