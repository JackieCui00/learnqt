#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newDocument();

    void open();

    void save();

    void saveAs();

    void print();

    void exit();

    void copy();

    void cut();

    void paste();

    void undo();

    void redo();

    void selectFont();

    void setFontBold(bool bold);

    void setFontUnderline(bool underline);

    void setFontItalic(bool italic);

    void about();

private:
    Ui::MainWindow *_ui;
    QString _current_file;
};

#endif // MAINWINDOW_H
