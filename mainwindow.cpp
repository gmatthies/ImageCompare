#include "mainwindow.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

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

    mDirectory = new QComboBox();
    mDirectory->addItem( QString( "<Select Base Directory>" ) );
    connect( mDirectory, SIGNAL( activated(int) ), this, SLOT( handleBaseDirectory() ) );

    mStartButton = new QPushButton( "Start Compare" );
    mStartButton->setEnabled( false );
    mStartButton->setFixedSize(150, 50);
    mStartButton->setFont( serifFont );
    connect( mStartButton, SIGNAL( clicked() ), this, SLOT( handleImageCompare() ) );

    mCompareStatus = new QLabel( "Processing 1 of X images" );
    serifFont.setPointSize( 12 );
    mCompareStatus->setFont( serifFont );
    mCompareStatus->setFixedHeight( 50 );
    mCompareStatus->hide();

    mMainLayout = new QVBoxLayout();

    mMainLayout->addWidget( mDirectory );
    mMainLayout->addWidget( mStartButton );
    mMainLayout->addWidget( mCompareStatus );
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
    mStartButton->setEnabled( true );
}

void MainWindow::handleImageCompare()
{
    QStringList paths;

    paths.push_back( mBaseDirectory );

    mFileList.clear();

    QStringList fileTypes;
    fileTypes << "*.jpg" << "*.png" << "*.bmp";

    QStringList imageFiles = findFilesRecursively( paths, fileTypes );

    for( int i = 0; i < imageFiles.count(); i++ )
    {
        mFileList.push_back( imageFiles[i] );
    }

    mCompareStatus->show();

    performImageCompare();

    int foo = 10;

    foo = foo + 1;

}

void MainWindow::performImageCompare()
{
    QString statusMsg = "Processing %1 of %2 files";

    // Aspect ratios for the two images to compare
    double imageBaseAR;
    double imageCompareAR;

    QDir workerDir;

    for( int i = 0; i < mFileList.size(); i++ )
    {
        QImage imageToCompare( mFileList[i] );

        mCompareStatus->setText( statusMsg.arg(i + 1).arg(mFileList.size()) );

        if( imageToCompare.isNull() )
        {
            continue;
        }

        imageBaseAR = (double)imageToCompare.width() / imageToCompare.height();

        for( int j = 0; j < mFileList.size(); j++ )
        {
            if( mFileList[i] == mFileList[j] )
            {
                continue;
            }

            QImage temp( mFileList[j] );

            if( temp.isNull() )
            {
                continue;
            }

            // Ok, we have two valid images, get second one's aspect ratio
            imageCompareAR = (double)temp.width() / temp.height();

            if( imageBaseAR != imageCompareAR )
            {
                continue;
            }

            // Ok, we have two images with the same aspect ratio, we can now compare
            QSize imageToCompareSize = imageToCompare.size();
            QSize tempSize = temp.size();

            QImage baseImage = imageToCompare;
            QImage compareImage = temp;

            bool isBaseLarger = false;

            if( imageToCompareSize != tempSize )
            {
                //Which is larger?
                int imageToComparePixels = imageToCompareSize.height() * imageToCompareSize.width();
                int tempPixels = tempSize.height() * tempSize.width();

                if( imageToComparePixels > tempPixels )
                {
                    baseImage = imageToCompare.scaled( tempSize );
                    isBaseLarger = true;
                }
                else
                {
                    compareImage = temp.scaled( imageToCompareSize );
                }
            }

            if( baseImage == compareImage )
            {
                // We have a match, we can get rid of one of them
                if( !isBaseLarger )
                {
                    // move larger one to the front
                    std::swap( mFileList[i], mFileList[j] );
                    imageToCompare = QImage( mFileList[j] );
                }

                workerDir.remove( mFileList[j] );
                mFileList.erase( mFileList.begin() + j );

                // Since we've shrunk the vector, we need to decrement j
                // So that we can get the new file which is now in the jth index
                j--;
            }
        }
    }
    mCompareStatus->setText( statusMsg.arg(mFileList.size()).arg(mFileList.size()) );
}
