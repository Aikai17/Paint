#include "MainWindow.h"
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow() {
    setupUI();
}

void MainWindow::onSave() {
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить рисунок", "", "Drawing Files (*.draw)");
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".draw")) {
            filename += ".draw";
        }
        drawingWidget->saveToFile(filename);
    }
}

void MainWindow::onLoad() {
    QString filename = QFileDialog::getOpenFileName(this, "Загрузить рисунок", "", "Drawing Files (*.draw)");
    if (!filename.isEmpty()) {
        drawingWidget->loadFromFile(filename);
    }
}

void MainWindow::onClear() {
    drawingWidget->clear();
}

void MainWindow::setupUI() {
    drawingWidget = new DrawingWidget;
    setCentralWidget(drawingWidget);

    QToolBar* toolbar = new QToolBar;
    addToolBar(toolbar);

    QAction* rectAction = new QAction("Прямоугольник", this);
    rectAction->setCheckable(true);
    QAction* triangleAction = new QAction("Треугольник", this);
    triangleAction->setCheckable(true);
    QAction* ellipseAction = new QAction("Эллипс", this);
    ellipseAction->setCheckable(true);

    QAction* connectAction = new QAction("Связь", this);
    connectAction->setCheckable(true);
    QAction* moveAction = new QAction("Переместить", this);
    moveAction->setCheckable(true);
    QAction* deleteAction = new QAction("Удалить", this);
    deleteAction->setCheckable(true);

    QAction* saveAction = new QAction("Сохранить", this);
    QAction* loadAction = new QAction("Загрузить", this);
    QAction* clearAction = new QAction("Очистить", this);

    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->addAction(rectAction);
    modeGroup->addAction(triangleAction);
    modeGroup->addAction(ellipseAction);
    modeGroup->addAction(connectAction);
    modeGroup->addAction(moveAction);
    modeGroup->addAction(deleteAction);
    modeGroup->setExclusive(true);

    toolbar->addAction(rectAction);
    toolbar->addAction(triangleAction);
    toolbar->addAction(ellipseAction);
    toolbar->addSeparator();
    toolbar->addAction(connectAction);
    toolbar->addAction(moveAction);
    toolbar->addAction(deleteAction);
    toolbar->addSeparator();
    toolbar->addAction(saveAction);
    toolbar->addAction(loadAction);
    toolbar->addAction(clearAction);

    connect(rectAction, &QAction::triggered, [this]() {
        drawingWidget->setMode(DrawingWidget::DrawShape);
        drawingWidget->setShapeType(Shape::Rectangle);
    });

    connect(triangleAction, &QAction::triggered, [this]() {
        drawingWidget->setMode(DrawingWidget::DrawShape);
        drawingWidget->setShapeType(Shape::Triangle);
    });

    connect(ellipseAction, &QAction::triggered, [this]() {
        drawingWidget->setMode(DrawingWidget::DrawShape);
        drawingWidget->setShapeType(Shape::Ellipse);
    });

    connect(connectAction, &QAction::triggered, [this]() {
        drawingWidget->setMode(DrawingWidget::Connect);
    });

    connect(moveAction, &QAction::triggered, [this]() {
        drawingWidget->setMode(DrawingWidget::Move);
    });

    connect(deleteAction, &QAction::triggered, [this]() {
        drawingWidget->setMode(DrawingWidget::Delete);
    });

    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoad);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClear);

    // Выбираем прямоугольник по умолчанию
    rectAction->trigger();

    setWindowTitle("Paint");
    resize(800, 600);
}
