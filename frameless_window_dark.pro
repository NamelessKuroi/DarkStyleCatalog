###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################

QT   += core gui concurrent sql charts xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH +="framelesswindow"

TARGET      =  DarkStyleCatalag
TEMPLATE    =  app

OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc
RCC_DIR = rcc

SOURCES     += main.cpp\
               framelesswindow/framelesswindow.cpp \
               framelesswindow/windowdragger.cpp \
               DarkStyle.cpp \
    Catalago/catalag.cpp \
    Catalago/Model/anime.cpp \
    Catalago/Model/state.cpp \
    Catalago/Model/kind.cpp \
    Catalago/Charts/statechartpie.cpp \
    Catalago/Charts/kindchartpie.cpp \
    Catalago/ReportGenerator/reportgenerator.cpp \
    Catalago/Controller/catalagmanager.cpp \
    Catalago/Controller/databasemanager.cpp \
    Catalago/Utils/display.cpp \
    Catalago/Utils/processaddress.cpp \
    Catalago/View/Form/FormAnime.cpp \
    Catalago/View/Form/FormKind.cpp \
    Catalago/View/Form/FormState.cpp \
    Catalago/View/Gallery/gallery.cpp \
    Catalago/View/Manager/kindmanager.cpp \
    Catalago/View/Manager/statemanager.cpp \
    Catalago/View/Manager/toolcatalg.cpp \
    Catalago/Utils/logger.cpp \
    Catalago/Utils/randomsplash.cpp



HEADERS     += \
               framelesswindow/framelesswindow.h \
               framelesswindow/windowdragger.h \
               DarkStyle.h \
    Catalago/catalag.h \
    Catalago/Model/anime.h \
    Catalago/Model/state.h \
    Catalago/Model/kind.h \
    Catalago/Charts/statechartpie.h \
    Catalago/Charts/kindchartpie.h \
    Catalago/ReportGenerator/reportgenerator.h \
    Catalago/Utils/singleton.h \
    Catalago/Utils/definitions.h \
    Catalago/Controller/catalagmanager.h \
    Catalago/Controller/databasemanager.h \
    Catalago/Utils/definitions.h \
    Catalago/Utils/display.h \
    Catalago/Utils/processaddress.h \
    Catalago/Utils/singleton.h \
    Catalago/View/Form/FormAnime.h \
    Catalago/View/Form/FormKind.h \
    Catalago/View/Form/FormState.h \
    Catalago/View/Gallery/gallery.h \
    Catalago/View/Manager/kindmanager.h \
    Catalago/View/Manager/statemanager.h \
    Catalago/View/Manager/toolcatalg.h \
    Catalago/Utils/logger.h \
    Catalago/Utils/randomsplash.h



FORMS       += \
               framelesswindow/framelesswindow.ui

RC_ICONS = IconCatalag.ico

RESOURCES   += darkstyle.qrc \
               framelesswindow.qrc

SUBDIRS += \
    Catalago/Catalago.pro

DISTFILES += \
    Catalago/DataBase/DataBase.db \
    Catalago/Icons/IconAdd.png \
    Catalago/Icons/IconCatalag.png \
    Catalago/Icons/IconCatalag1.png \
    Catalago/Icons/IconDelete.png \
    Catalago/Icons/IconEdit.png \
    Catalago/Icons/IconKinds.png \
    Catalago/Icons/IconList.png \
    Catalago/StyleSheet/rc/branch_closed-on.png \
    Catalago/StyleSheet/rc/branch_closed.png \
    Catalago/StyleSheet/rc/branch_open-on.png \
    Catalago/StyleSheet/rc/branch_open.png \
    Catalago/StyleSheet/rc/checkbox_checked.png \
    Catalago/StyleSheet/rc/checkbox_checked_disabled.png \
    Catalago/StyleSheet/rc/checkbox_checked_focus.png \
    Catalago/StyleSheet/rc/checkbox_indeterminate.png \
    Catalago/StyleSheet/rc/checkbox_indeterminate_disabled.png \
    Catalago/StyleSheet/rc/checkbox_indeterminate_focus.png \
    Catalago/StyleSheet/rc/checkbox_unchecked.png \
    Catalago/StyleSheet/rc/checkbox_unchecked_disabled.png \
    Catalago/StyleSheet/rc/checkbox_unchecked_focus.png \
    Catalago/StyleSheet/rc/close-hover.png \
    Catalago/StyleSheet/rc/close-pressed.png \
    Catalago/StyleSheet/rc/close.png \
    Catalago/StyleSheet/rc/down_arrow.png \
    Catalago/StyleSheet/rc/down_arrow_disabled.png \
    Catalago/StyleSheet/rc/Hmovetoolbar.png \
    Catalago/StyleSheet/rc/Hsepartoolbar.png \
    Catalago/StyleSheet/rc/left_arrow.png \
    Catalago/StyleSheet/rc/left_arrow_disabled.png \
    Catalago/StyleSheet/rc/radio_checked.png \
    Catalago/StyleSheet/rc/radio_checked_disabled.png \
    Catalago/StyleSheet/rc/radio_checked_focus.png \
    Catalago/StyleSheet/rc/radio_unchecked.png \
    Catalago/StyleSheet/rc/radio_unchecked_disabled.png \
    Catalago/StyleSheet/rc/radio_unchecked_focus.png \
    Catalago/StyleSheet/rc/right_arrow.png \
    Catalago/StyleSheet/rc/right_arrow_disabled.png \
    Catalago/StyleSheet/rc/sizegrip.png \
    Catalago/StyleSheet/rc/stylesheet-branch-end.png \
    Catalago/StyleSheet/rc/stylesheet-branch-more.png \
    Catalago/StyleSheet/rc/stylesheet-vline.png \
    Catalago/StyleSheet/rc/transparent.png \
    Catalago/StyleSheet/rc/undock.png \
    Catalago/StyleSheet/rc/up_arrow.png \
    Catalago/StyleSheet/rc/up_arrow_disabled.png \
    Catalago/StyleSheet/rc/Vmovetoolbar.png \
    Catalago/StyleSheet/rc/Vsepartoolbar.png \
    Catalago/StyleSheet/icon_branch_closed.png \
    Catalago/StyleSheet/icon_branch_end.png \
    Catalago/StyleSheet/icon_branch_more.png \
    Catalago/StyleSheet/icon_branch_open.png \
    Catalago/StyleSheet/icon_checkbox_checked.png \
    Catalago/StyleSheet/icon_checkbox_checked_disabled.png \
    Catalago/StyleSheet/icon_checkbox_checked_pressed.png \
    Catalago/StyleSheet/icon_checkbox_indeterminate.png \
    Catalago/StyleSheet/icon_checkbox_indeterminate_disabled.png \
    Catalago/StyleSheet/icon_checkbox_indeterminate_pressed.png \
    Catalago/StyleSheet/icon_checkbox_unchecked.png \
    Catalago/StyleSheet/icon_checkbox_unchecked_disabled.png \
    Catalago/StyleSheet/icon_checkbox_unchecked_pressed.png \
    Catalago/StyleSheet/icon_close.png \
    Catalago/StyleSheet/icon_radiobutton_checked.png \
    Catalago/StyleSheet/icon_radiobutton_checked_disabled.png \
    Catalago/StyleSheet/icon_radiobutton_checked_pressed.png \
    Catalago/StyleSheet/icon_radiobutton_unchecked.png \
    Catalago/StyleSheet/icon_radiobutton_unchecked_disabled.png \
    Catalago/StyleSheet/icon_radiobutton_unchecked_pressed.png \
    Catalago/StyleSheet/icon_restore.png \
    Catalago/StyleSheet/icon_undock.png \
    Catalago/StyleSheet/icon_vline.png \
    Catalago/StyleSheet/DarkStyle.qss \
    Catalago/StyleSheet/style.qss \
    Catalago/ReportGenerator/style.css



