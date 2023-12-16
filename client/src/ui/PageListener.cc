#include <Havoc.h>
#include <ui/PageListener.h>

HavocPageListener::HavocPageListener() {
    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "PageListener" ) );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( QString::fromUtf8( "gridLayout" ) );

    ActiveLabel = new QLabel( this );
    ActiveLabel->setObjectName( QString::fromUtf8("LabelDisplayListenerActive" ) );
    ActiveLabel->setProperty( "HcLabelDisplay", "true" );

    horizontalSpacer  = new QSpacerItem( 1157, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ButtonNewListener = new QPushButton( this );
    ButtonNewListener->setObjectName( QString::fromUtf8( "ButtonNewListener" ) );
    ButtonNewListener->setProperty( "HcButton", "true" );

    Splitter = new QSplitter( this );
    Splitter->setObjectName( QString::fromUtf8( "Splitter" ) );
    Splitter->setOrientation( Qt::Vertical );

    TableWidget = new QTableWidget( Splitter );
    TableWidget->setObjectName( QString::fromUtf8( "TableWidget" ) );

    TitleName   = new QTableWidgetItem( "Name" );
    TitleType   = new QTableWidgetItem( "Type" );
    TitleHost   = new QTableWidgetItem( "Host" );
    TitlePort   = new QTableWidgetItem( "Port" );
    TitleStatus = new QTableWidgetItem( "Status" );

    if ( TableWidget->columnCount() < 5 ) {
        TableWidget->setColumnCount( 5 );
    }

    TableWidget->setHorizontalHeaderItem( 0, TitleName   );
    TableWidget->setHorizontalHeaderItem( 1, TitleType   );
    TableWidget->setHorizontalHeaderItem( 2, TitleHost   );
    TableWidget->setHorizontalHeaderItem( 3, TitlePort   );
    TableWidget->setHorizontalHeaderItem( 4, TitleStatus );

    /* table settings */
    TableWidget->setEnabled( true );
    TableWidget->setShowGrid( false );
    TableWidget->setSortingEnabled( false );
    TableWidget->setWordWrap( true );
    TableWidget->setCornerButtonEnabled( true );
    TableWidget->horizontalHeader()->setVisible( true );
    TableWidget->setSelectionBehavior( QAbstractItemView::SelectRows );
    TableWidget->setContextMenuPolicy( Qt::CustomContextMenu );
    TableWidget->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeMode::Stretch );
    TableWidget->horizontalHeader()->setStretchLastSection( true );
    TableWidget->verticalHeader()->setVisible( false );
    TableWidget->setFocusPolicy( Qt::NoFocus );
    
    TabWidget = new QTabWidget( Splitter );
    TabWidget->setObjectName( QString::fromUtf8( "TabWidget" ) );
    TabWidget->setMovable( true );
    TabWidget->setTabsClosable( true );
    TabWidget->tabBar()->setProperty( "HcTab", "true" );

    Splitter->addWidget( TableWidget );
    Splitter->addWidget( TabWidget );
    Splitter->handle( 1 )->setEnabled( false ); /* disabled by default */

    gridLayout->addWidget( ButtonNewListener, 0, 0, 1, 1 );
    gridLayout->addItem( horizontalSpacer, 0, 1, 1, 1 );
    gridLayout->addWidget( ActiveLabel, 0, 2, 1, 1 );
    gridLayout->addWidget( Splitter, 1, 0, 1, 3 );

    retranslateUi();

    QObject::connect( ButtonNewListener, &QPushButton::clicked, this, &HavocPageListener::buttonAddListener );
    QObject::connect( TableWidget, &QTableWidget::customContextMenuRequested, this, &HavocPageListener::handleListenerContextMenu );
    QObject::connect( TabWidget->tabBar(), &QTabBar::tabCloseRequested, this, &HavocPageListener::tabCloseRequested );

    QMetaObject::connectSlotsByName( this );
}

auto HavocPageListener::retranslateUi() -> void {
    setStyleSheet( Havoc->getStyleSheet() );

    setWindowTitle( "PageListener" );
    ActiveLabel->setText( "Active: 0" );
    ButtonNewListener->setText( "Add Listener" );
}

auto HavocPageListener::buttonAddListener() -> void {
    auto Dialog = new HavocListener;

    /* add registered protocols
     * to the dialog listener */
    for ( auto& p : Protocols ) {
        Dialog->addProtocol( p );
    }

    Dialog->start();

    delete Dialog;
}

auto HavocPageListener::addListener(
    const json& data
) -> void {
    auto name     = QString();
    auto type     = QString();
    auto host     = QString();
    auto port     = QString();
    auto status   = QString();
    auto listener = new Listener();

    if ( data.contains( "name" ) ) {
        if ( data[ "name" ].is_string() ) {
            name = data[ "name" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"name\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"name\" is not found" );
        return;
    }

    if ( data.contains( "protocol" ) ) {
        if ( data[ "protocol" ].is_string() ) {
            type = data[ "protocol" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"protocol\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"protocol\" is not found" );
        return;
    }

    if ( data.contains( "host" ) ) {
        if ( data[ "host" ].is_string() ) {
            host = data[ "host" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"host\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"host\" is not found" );
        return;
    }

    if ( data.contains( "port" ) ) {
        if ( data[ "port" ].is_string() ) {
            port = data[ "port" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"port\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"port\" is not found" );
        return;
    }

    if ( data.contains( "status" ) ) {
        if ( data[ "status" ].is_string() ) {
            status = data[ "status" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"status\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"status\" is not found" );
        return;
    }

    listener->Name   = new QTableWidgetItem( name );
    listener->Type   = new QTableWidgetItem( type );
    listener->Host   = new QTableWidgetItem( host );
    listener->Port   = new QTableWidgetItem( port );
    listener->Status = new QTableWidgetItem( status );
    listener->Logger = new QTextEdit();

    listener->Name->setFlags( listener->Name->flags() ^ Qt::ItemIsEditable );
    listener->Type->setFlags( listener->Type->flags() ^ Qt::ItemIsEditable );
    listener->Host->setFlags( listener->Host->flags() ^ Qt::ItemIsEditable );
    listener->Port->setFlags( listener->Port->flags() ^ Qt::ItemIsEditable );
    listener->Status->setFlags( listener->Status->flags() ^ Qt::ItemIsEditable );
    listener->Logger->setProperty( "HcConsole", "true" );
    listener->Logger->setReadOnly( true );

    if ( TableWidget->rowCount() < 1 ) {
        TableWidget->setRowCount( 1 );
    } else {
        TableWidget->setRowCount( TableWidget->rowCount() + 1 );
    }

    const bool isSortingEnabled = TableWidget->isSortingEnabled();
    TableWidget->setSortingEnabled( false );
    TableWidget->setItem( TableWidget->rowCount() - 1, 0, listener->Name );
    TableWidget->setItem( TableWidget->rowCount() - 1, 1, listener->Type );
    TableWidget->setItem( TableWidget->rowCount() - 1, 2, listener->Host );
    TableWidget->setItem( TableWidget->rowCount() - 1, 3, listener->Port );
    TableWidget->setItem( TableWidget->rowCount() - 1, 4, listener->Status );
    TableWidget->setSortingEnabled( isSortingEnabled );

    TableEntries.push_back( listener );

    /* increase the number of listeners */
    ListenersRunning++;

    updateListenersRunningLabel( ListenersRunning );
}

auto HavocPageListener::updateListenersRunningLabel(
    int value
) const -> void {
    ActiveLabel->setText( QString( "Active: %1" ).arg( value ) );
}

auto HavocPageListener::addListenerLog(
    const std::string& name,
    const std::string& log
) -> void {
    for ( auto& listener : TableEntries ) {
        if ( listener->Name->text().toStdString() == name ) {
            listener->Logger->append( log.c_str() );
            break;
        }
    }
}

auto HavocPageListener::handleListenerContextMenu(
    const QPoint& pos
) -> void {
    auto Menu = new QMenu( this );
    auto Name = QString();

    /* check if we point to a session table item/agent */
    if ( ! TableWidget->itemAt( pos ) ) {
        return;
    }

    Name = TableWidget->item( TableWidget->currentRow(), 0 )->text();

    Menu->addAction( "Logs"    );
    Menu->addAction( "Stop"    );
    Menu->addAction( "Restart" );

    if ( auto action = Menu->exec( TableWidget->horizontalHeader()->viewport()->mapToGlobal( pos ) ) ) {
        if ( action->text().compare( "Logs" ) == 0 ) {
            for ( auto& listener : TableEntries ) {
                if ( listener->Name->text().compare( Name ) == 0 ) {

                    if ( TabWidget->count() == 0 ) {
                        Splitter->setSizes( QList<int>() << 200 << 220 );
                        Splitter->handle( 1 )->setEnabled( true );
                        Splitter->handle( 1 )->setCursor( Qt::SplitVCursor );
                    }

                    TabWidget->addTab( listener->Logger, "[Logger] " + listener->Name->text() );

                    break;
                }
            }
        } else {
            spdlog::debug( "[ERROR] invalid action from selected listener menu" );
        }
    }

}

auto HavocPageListener::tabCloseRequested(
    int index
) const -> void {
    if ( index == -1 ) {
        return;
    }

    TabWidget->removeTab( index );

    if ( TabWidget->count() == 0 ) {
        Splitter->setSizes( QList<int>() << 0 );
        Splitter->handle( 1 )->setEnabled( false );
        Splitter->handle( 1 )->setCursor( Qt::ArrowCursor );
    }
}