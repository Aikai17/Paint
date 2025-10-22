#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DrawingWidget.h"
#include <QMainWindow>
#include <QToolBar>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private slots:
    void onSave();
    void onLoad();
    void onClear();

private:
    void setupUI();

    DrawingWidget* drawingWidget;
};

#endif // MAINWINDOW_H
