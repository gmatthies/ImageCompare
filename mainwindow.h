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
        void        handleBaseDirectory();

    protected:
        QStringList findFilesRecursively ( QStringList paths, QStringList fileTypes );

        QString         mBaseDirectory;

        QVBoxLayout     *mMainLayout;
        QPushButton     *mStartButton;
        QComboBox       *mDirectory;

};

#endif // MAINWINDOW_H
