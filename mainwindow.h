#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtGui>
#include <QDir>
#include <QMainWindow>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget * parent = 0);
        ~MainWindow();

    protected slots:
        void        handleImageCompare();

    protected:
        QStringList findFilesRecursively ( QStringList paths, QStringList fileTypes );

        QVBoxLayout *mMainLayout;
        QPushButton *mStartButton;

};

#endif // MAINWINDOW_H
