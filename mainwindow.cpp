#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QFontDialog>
#include <tuple>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    this->setCentralWidget(_ui->textEdit);

    connect(_ui->actionNew, &QAction::triggered, this, &MainWindow::newDocument);
    connect(_ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(_ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(_ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(_ui->actionPrint, &QAction::triggered, this, &MainWindow::print);
    connect(_ui->actionExit, &QAction::triggered, this, &MainWindow::exit);
    connect(_ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(_ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(_ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(_ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(_ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(_ui->actionFont, &QAction::triggered, this, &MainWindow::selectFont);
    connect(_ui->actionBold, &QAction::triggered, this, &MainWindow::setFontBold);
    connect(_ui->actionUnderline, &QAction::triggered, this, &MainWindow::setFontUnderline);
    connect(_ui->actionItalic, &QAction::triggered, this, &MainWindow::setFontItalic);
    connect(_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);

    // Disable menu actions for unavailable features
#if !defined(QT_PRINTSUPPORT_LIB) || !QT_CONFIG(printer)
    _ui->actionPrint->setEnabled(false);
#endif

#if !QT_CONFIG(clipboard)
    _ui->actionCut->setEnabled(false);
    _ui->actionCopy->setEnabled(false);
    _ui->actionPaste->setEnabled(false);
#endif
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::newDocument() {
    this->_current_file.clear();
    this->_ui->textEdit->setText(QString());
}

void MainWindow::open() {
    QString file_name = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(file_name);
    this->_current_file = file_name;

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    this->setWindowTitle(file_name);
    QTextStream in(&file);
    QString text = in.readAll();
    this->_ui->textEdit->setText(text);
    file.close();
}

void MainWindow::save() {
    QString file_name;
    // if we done't have a filename from before, get one
    if (this->_current_file.isEmpty()) {
        file_name = QFileDialog::getSaveFileName(this, "Save");
        this->_current_file = file_name;
    } else {
        file_name = this->_current_file;
    }

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    this->setWindowTitle(file_name);
    QTextStream out(&file);
    QString text = this->_ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MainWindow::saveAs() {
    QString file_name = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(file_name);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    this->_current_file = file_name;
    this->setWindowTitle(file_name);

    QTextStream out(&file);
    QString text = this->_ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MainWindow::print() {
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter print_dev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&print_dev, this);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
#endif // QT_COPNFIG(printdialog)
    this->_ui->textEdit->print(&print_dev);
#endif // QT_CONFIG(printer)
}

void MainWindow::exit() {
    QCoreApplication::quit();
}

void MainWindow::copy() {
#if QT_CONFIG(clipboard)
    this->_ui->textEdit->copy();
#endif
}

void MainWindow::cut() {
#if QT_CONFIG(clipboard)
    this->_ui->textEdit->cut();
#endif
}

void MainWindow::paste() {
#if QT_CONFIG(clipboard)
    this->_ui->textEdit->paste();
#endif
}

void MainWindow::undo() {
    this->_ui->textEdit->undo();
}

void MainWindow::redo() {
    this->_ui->textEdit->redo();
}

void MainWindow::selectFont() {
    bool font_selected = false;
    QFont font = QFontDialog::getFont(&font_selected, this);
    if (font_selected) {
        this->_ui->textEdit->setFont(font);
    }
}

void MainWindow::setFontBold(bool bold) {
    std::ignore = bold;
}

void MainWindow::setFontUnderline(bool underline) {
    std::ignore = underline;
}

void MainWindow::setFontItalic(bool italic) {
    std::ignore = italic;
}

void MainWindow::about() {
}
