QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithmen.cpp \
    booking.cpp \
    buchungsdetails.cpp \
    check.cpp \
    customer.cpp \
    flightbooking.cpp \
    hotelbooking.cpp \
    main.cpp \
    meldungen.cpp \
    rentalcarreservation.cpp \
    settings.cpp \
    trainticket.cpp \
    travel.cpp \
    travelagency.cpp \
    travelagencyui.cpp

HEADERS += \
    booking.h \
    buchungsdetails.h \
    check.h \
    customer.h \
    flightbooking.h \
    graph.h \
    hotelbooking.h \
    meldungen.h \
    rentalcarreservation.h \
    settings.h \
    trainticket.h \
    travel.h \
    travelagency.h \
    travelagencyui.h

FORMS += \
    buchungsdetails.ui \
    meldungen.ui \
    settings.ui \
    travelagencyui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Graphic_Elements.qrc
