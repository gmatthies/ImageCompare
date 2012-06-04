#include "mainwindow.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Set size
    int aWidth;
    int aHeight;
    int aScreenWidth;
    int aScreenHeight;


    QDesktopWidget *desktop = new QDesktopWidget();

    //Create main widget for the window
    QWidget *centralWidget  = new QWidget();

    //Get size of the "first" monitor. This will ensure
    //the window is displayed centered on one monitor
    aScreenWidth  = desktop->screenGeometry(1).width();
    aScreenHeight = desktop->screenGeometry(1).height();

    //Set width and height to 60% of screen's width and height
    aWidth  = 0.25 * aScreenWidth;
    aHeight = 0.25 * aScreenHeight;

    //set size
    resize(aWidth, aHeight);

    QFont serifFont("Times", 14, QFont::Normal);

    mStartButton = new QPushButton( "Start Compare" );
    mStartButton->setFixedSize(150, 50);
    mStartButton->setFont( serifFont );
    connect( mStartButton, SIGNAL( clicked() ), this, SLOT( handleImageCompare() ) );

    mDirectory = new QComboBox();
    mDirectory->addItem( QString( "<Select Base Directory>" ) );
    connect( mDirectory, SIGNAL( activated(int) ), this, SLOT( handleBaseDirectory() ) );

    mMainLayout = new QVBoxLayout();

    mMainLayout->addWidget( mDirectory );
    mMainLayout->addWidget( mStartButton );
    mMainLayout->setAlignment( Qt::AlignHCenter );
    mMainLayout->setSpacing( 30 );

    centralWidget->setLayout(mMainLayout);

    setCentralWidget(centralWidget);

}

MainWindow::~MainWindow()
{
}

QStringList MainWindow::findFilesRecursively ( QStringList paths, QStringList fileTypes )
{
    if ( fileTypes.isEmpty() )
    {
        fileTypes.append("*");
    }
    QStringList result;
    QStringList more;

    QStringList::Iterator it;

    for( uint i = 0 ; i < paths.size() ; i++ )
    {
        // inefficient...whatever
        QDir dir( paths[i] );

        more = dir.entryList( fileTypes );

        for ( it = more.begin() ; it != more.end() ; ++it )
        {
            result.append( paths[i] + "/" + *it );
        }

        // reg exp in next line excludes . and .. dirs (and .* actually)
        more = dir.entryList( QDir::Dirs ).filter( QRegExp( "[^.]" ) );

        for( it = more.begin() ; it != more.end() ; ++it )
        {
            *it = paths[i] + "/" + *it;
        }

        more = findFilesRecursively( more, fileTypes );

        for( it = more.begin() ; it != more.end() ; ++it )
        {
            result.append( *it );
        }
    }
    return result; // yields absolute paths
}

void MainWindow::handleBaseDirectory()
{
    mBaseDirectory = QFileDialog::getExistingDirectory( this, tr( "Base Directory" ) );
    std::cout << "hello";
}

void MainWindow::handleImageCompare()
{
    QStringList paths;

    QDir dir;

    paths.push_back( dir.currentPath() );

    QStringList fileTypes;
    fileTypes << "*.jpg" << "*.png" << "*.bmp";

    QStringList imageFiles = findFilesRecursively( paths, fileTypes );

    int foo = 10;

    foo = foo + 1;

}

