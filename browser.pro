TEMPLATE = app
TARGET = browser
QT += webkitwidgets network widgets printsupport

qtHaveModule(uitools):!embedded: QT += uitools
else: DEFINES += QT_NO_UITOOLS

FORMS += \
    addbookmarkdialog.ui \
    bookmarks.ui \
    cookies.ui \
    cookiesexceptions.ui \
    downloaditem.ui \
    downloads.ui \
    history.ui \
    passworddialog.ui \
    proxy.ui \
    settings.ui \
    adblock/adblockdialog.ui

HEADERS += \
    autosaver.h \
    bookmarks.h \
    browserapplication.h \
    browsermainwindow.h \
    chasewidget.h \
    cookiejar.h \
    downloadmanager.h \
    edittableview.h \
    edittreeview.h \
    history.h \
    modelmenu.h \
    networkaccessmanager.h \
    searchlineedit.h \
    settings.h \
    squeezelabel.h \
    tabwidget.h \
    toolbarsearch.h \
    urllineedit.h \
    webview.h \
    xbel.h \
    jsapi.h \
    adblock/adblocksubscription.h \
    adblock/adblockschemeaccesshandler.h \
    adblock/adblockrule.h \
    adblock/adblockpage.h \
    adblock/adblocknetwork.h \
    adblock/adblockmodel.h \
    adblock/adblockmanager.h \
    adblock/adblockdialog.h \
    adblock/adblockblockednetworkreply.h \
    treesortfilterproxymodel.h \
    schemeaccesshandler.h

SOURCES += \
    autosaver.cpp \
    bookmarks.cpp \
    browserapplication.cpp \
    browsermainwindow.cpp \
    chasewidget.cpp \
    cookiejar.cpp \
    downloadmanager.cpp \
    edittableview.cpp \
    edittreeview.cpp \
    history.cpp \
    modelmenu.cpp \
    networkaccessmanager.cpp \
    searchlineedit.cpp \
    settings.cpp \
    squeezelabel.cpp \
    tabwidget.cpp \
    toolbarsearch.cpp \
    urllineedit.cpp \
    webview.cpp \
    xbel.cpp \
    main.cpp \
    jsapi.cpp \
    adblock/adblocksubscription.cpp \
    adblock/adblockschemeaccesshandler.cpp \
    adblock/adblockrule.cpp \
    adblock/adblockpage.cpp \
    adblock/adblocknetwork.cpp \
    adblock/adblockmodel.cpp \
    adblock/adblockmanager.cpp \
    adblock/adblockdialog.cpp \
    adblock/adblockblockednetworkreply.cpp \
    treesortfilterproxymodel.cpp \
    schemeaccesshandler.cpp

RESOURCES += data/data.qrc htmls/htmls.qrc

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

win32 {
   RC_FILE = browser.rc
}

mac {
    ICON = browser.icns
    QMAKE_INFO_PLIST = Info_mac.plist
    TARGET = Browser

    # No 64-bit Flash on Mac, so build the browser 32-bit
    contains(QT_CONFIG, x86) {
        CONFIG -= x86_64
        CONFIG += x86
    }
    contains(QT_CONFIG, ppc) {
        CONFIG -= ppc64
        CONFIG += ppc
    }
}

wince*: {
    DEPLOYMENT_PLUGIN += qjpeg qgif
}

EXAMPLE_FILES = Info_mac.plist browser.icns browser.ico browser.rc

# install
target.path = /bedbrowser
INSTALLS += target

OTHER_FILES += \
    htmls/home.html \
    adblock/adblock.pri \
    README.md \
    Info_mac.plist \
    browser.rc \
    browser.pro.user \
    browser.ico
