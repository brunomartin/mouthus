#include "mouthus_main_window.h"

#include "mouthus_core.h"
#include "mouthus_network_manager.h"

#include <QKeyEvent>
#include <QDebug>

MouthusMainWindow::MouthusMainWindow(MouthusCore* core) :
    _core(core),
    _tryCount(0),
    _currentItem(0),
    _gameState(GameStopped),
    _timeMode(TimeModeNotWaiting),
    _currentAnsweringPosition(0),
    _lastTry(false),
    _roundResult(RoundLost),
    _blinkingLetter(0)
{
    Q_INIT_RESOURCE(mouthus);
    
    _ui.setupUi(this);

    _ui.tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    _ui.tableWidget->setFocusPolicy(Qt::NoFocus);
    _ui.tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    _ui.tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _ui.tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    setStatusBar(new QStatusBar);
    
    QSignalMapper *letterCountSignalMapper = new QSignalMapper;
    QActionGroup* letterCountActionGroup = new QActionGroup(this);
    
    int defaultLetterCount = 10;
    QAction* defaultLetterCountAction;
    
    for(int i=_core->letterCountMin();i<=_core->letterCountMax();i++)
    {
        QAction* letterCountAction = _ui.letterCountMenu->addAction(QString::number(i));
        connect(letterCountAction, SIGNAL(triggered()), letterCountSignalMapper, SLOT(map()));
        letterCountSignalMapper->setMapping(letterCountAction, i);
        letterCountActionGroup->addAction(letterCountAction);
        letterCountAction->setCheckable(true);
        if(i==defaultLetterCount)
            defaultLetterCountAction = letterCountAction;
    }
    
    connect(letterCountSignalMapper, SIGNAL(mapped(int)), this, SLOT(setLetterCount(int)));

    QActionGroup* timeModeActionGroup = new QActionGroup(this);
    
    QAction* timeModeWaitingAction = _ui.timeModeMenu->addAction(tr("Attente des resultats"));
    QAction* timeModeNotWaitingAction = _ui.timeModeMenu->addAction(tr("Pas d'attente des resultats"));
    
    timeModeWaitingAction->setCheckable(true);
    timeModeNotWaitingAction->setCheckable(true);

    this->connect(timeModeWaitingAction, SIGNAL(triggered()), SLOT(setTimeModeWaiting()));
    this->connect(timeModeNotWaitingAction, SIGNAL(triggered()), SLOT(setTimeModeNotWaiting()));
    
    timeModeActionGroup->addAction(timeModeWaitingAction);
    timeModeActionGroup->addAction(timeModeNotWaitingAction);
        
    timeModeWaitingAction->trigger();
    
    setTryCount(7);
    
    this->connect(&_answeringTimer, SIGNAL(timeout()), SLOT(colorizeNextLetter()));
    this->connect(&_winningRoundTimer, SIGNAL(timeout()), SLOT(blinkNextLetter()));
   
    defaultLetterCountAction->trigger();
    
    _networkManager = new MouthusNetworkManager;
    _ui.label->connect(_networkManager, SIGNAL(httpReplyReceived(const QString &)), SLOT(setText(const QString &)));
}

MouthusMainWindow::~MouthusMainWindow()
{
    _answeringTimer.stop();
}

void MouthusMainWindow::setTryCount(int tryCount)
{
    _tryCount = tryCount;
}

void MouthusMainWindow::setLetterCount(int letterCount)
{
    _core->setLetterCount(letterCount);
    startGame();
}

void MouthusMainWindow::startGame()
{
    setGameState(GameStarted);
    newRound();
}

void MouthusMainWindow::stopGame()
{
    setGameState(GameStopped);
}

void MouthusMainWindow::keyPressEvent(QKeyEvent* event)
{
    if(_timeMode == TimeModeWaiting && _gameState == GameGivingAnswers)
        return QMainWindow::keyPressEvent(event);
    
    switch (_gameState) {
        case GameStopped:
        case GameGivingAnswers:
            return QMainWindow::keyPressEvent(event);
            break;
            
        case GameWaintingForEnterKey:
            if(event->key() == Qt::Key_Enter ||
               event->key() == Qt::Key_Return)
            {
                if(1)
                {
                    for(int i=0;i<_tryCount;i++)
                    {
                        QCheckBox* checkBox = dynamic_cast<QCheckBox*>(_ui.isCorrectWordLayout->itemAt(i)->widget());
                        Q_ASSERT(checkBox);

                        if(checkBox->isEnabled())
                        {
                            QString url = "http://brunomartin.dtdns.net:2180/mouthus/add_new_word_statistics.php";
                            url += "?accessCode=" + QString("1234abcd");
                            //url += "&ipAddress=" + QString::fromStdString();
                            url += "&word=" + wordAtRow(i).toLower();
                            url += "&isOk=" + QString::number(checkBox->checkState()==Qt::Checked);
                            url += "&difficulty=" + QString::number(2);
                            _networkManager->setUrl(url);
                            _networkManager->sendHttpRequest();
                        }
                    }
                }
                
                _winningRoundTimer.stop();
                _currentItem = 0;
                statusBar()->clearMessage();
                setGameState(GameStarted);
                newRound();
                return QMainWindow::keyPressEvent(event);
            }
            break;
            
        case GameStarted:
            switch (event->key()) {
                case Qt::Key_A:
                    enterLetter("A");
                    break;
                case Qt::Key_B:
                    enterLetter("B");
                    break;
                case Qt::Key_C:
                    enterLetter("C");
                    break;
                case Qt::Key_D:
                    enterLetter("D");
                    break;
                case Qt::Key_E:
                    enterLetter("E");
                    break;
                case Qt::Key_F:
                    enterLetter("F");
                    break;
                case Qt::Key_G:
                    enterLetter("G");
                    break;
                case Qt::Key_H:
                    enterLetter("H");
                    break;
                case Qt::Key_I:
                    enterLetter("I");
                    break;
                case Qt::Key_J:
                    enterLetter("J");
                    break;
                case Qt::Key_K:
                    enterLetter("K");
                    break;
                case Qt::Key_L:
                    enterLetter("L");
                    break;
                case Qt::Key_M:
                    enterLetter("M");
                    break;
                case Qt::Key_N:
                    enterLetter("N");
                    break;
                case Qt::Key_O:
                    enterLetter("O");
                    break;
                case Qt::Key_P:
                    enterLetter("P");
                    break;
                case Qt::Key_Q:
                    enterLetter("Q");
                    break;
                case Qt::Key_R:
                    enterLetter("R");
                    break;
                case Qt::Key_S:
                    enterLetter("S");
                    break;
                case Qt::Key_T:
                    enterLetter("T");
                    break;
                case Qt::Key_U:
                    enterLetter("U");
                    break;
                case Qt::Key_V:
                    enterLetter("V");
                    break;
                case Qt::Key_W:
                    enterLetter("W");
                    break;
                case Qt::Key_X:
                    enterLetter("X");
                    break;
                case Qt::Key_Y:
                    enterLetter("Y");
                    break;
                case Qt::Key_Z:
                    enterLetter("Z");
                    break;
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    validateWord();
                    break;
                case Qt::Key_Left:
                    moveCurrentItem(Left);
                    break;
                case Qt::Key_Right:
                    moveCurrentItem(Right);
                    break;
                case Qt::Key_Backspace:
                    if(_currentItem->column() != _ui.tableWidget->columnCount()-1)
                        moveCurrentItem(Left);
                    else if(_currentItem->data(Qt::DisplayRole) == ".")
                        moveCurrentItem(Left);
                    enterLetter(".");
                    break;
                default:
                    return QMainWindow::keyPressEvent(event);
                    break;
            }
            
            if(event->key() != Qt::Key_Enter &&
               event->key() != Qt::Key_Return &&
               event->key() != Qt::Key_Left &&
               event->key() != Qt::Key_Right &&
               event->key() != Qt::Key_Backspace)
            {
                moveCurrentItem(Right);
            }
            
            return QMainWindow::keyPressEvent(event);
            break;
            
        case GamePaused:
            return QMainWindow::keyPressEvent(event);
            break;
            
        default: Q_ASSERT(0);
            break;
    }
}

void MouthusMainWindow::enterLetter(const QString & letter)
{
    Q_ASSERT(_currentItem);
    _currentItem->setData(Qt::DisplayRole, letter);
        
    return;
}

void MouthusMainWindow::newRound()
{
    _guessedPosition.clear();
    _lastTry = false;
    _currentItem = 0;
    _winningRoundTimer.stop();
    _roundResult = RoundLost;

    _ui.tableWidget->clear();
    
    while (!_ui.isCorrectWordLayout->isEmpty()) {
        QWidget* widget = _ui.isCorrectWordLayout->itemAt(0)->widget();
        _ui.isCorrectWordLayout->removeWidget(widget);
        delete widget;
    }
    
    
    _core->newWordToGuess();
    
    qDebug() << "word to guess : " << QString::fromStdString(_core->wordToGuess());
    
    int letterCount = _core->letterCount();
    
    _ui.tableWidget->setColumnCount(letterCount);
    _ui.tableWidget->setRowCount(_tryCount);
    
    for(int i=0;i<_tryCount;i++)
    {
        for(int j=0;j<letterCount;j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem;            
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setBackgroundColor(Qt::white);
            _ui.tableWidget->setItem(i, j, item);
        }
        QCheckBox* checkBox = new QCheckBox;
        checkBox->setCheckState(Qt::Unchecked);
        checkBox->setDisabled(true);
        _ui.isCorrectWordLayout->addWidget(checkBox);
        checkBox->setMinimumSize(30, 30);
    }
    _guessedPosition.insert(0);
    newLine();
    
    _ui.tableWidget->setFixedSize(_ui.tableWidget->horizontalHeader()->length(), _ui.tableWidget->verticalHeader()->length());
}

void MouthusMainWindow::validateWord()
{
    Q_ASSERT(_currentItem);
    
    setGameState(GameGivingAnswers);
   
    const int & itemRow = _currentItem->row();
    const int & rowCount = _ui.tableWidget->rowCount();
    const int & colCount = _ui.tableWidget->columnCount();

    QString guessWord;
    
    for(int i=0;i<colCount;i++)
    {
        QTableWidgetItem* item = _ui.tableWidget->item(itemRow,i);
        guessWord.append(item->data(Qt::DisplayRole).toString());
    }
    
    guessWord.remove(".");
    
    _core->guessWord(guessWord.toLower().toStdString());
    
    const MouthusGuessResult & guessResult = _core->lastGuessResults();
        
    _lastTry = itemRow == rowCount-1;
    
    _roundResult = guessResult.isWriteWord?RoundWon:RoundLost;

    if(_timeMode == TimeModeNotWaiting)
    {
        if(guessResult.isLetterCountOk && guessResult.wordExists && guessResult.isFirstLetterOk)
        {
            for(int i=0;i<colCount;i++)
                colorizeLetter(i);
        }
        else if(!_lastTry)
        {
            for(int i=0;i<colCount;i++)
            {
                QTableWidgetItem* item = _ui.tableWidget->item(itemRow,i);
                item->setBackgroundColor(Qt::white);
            }
        }

        if(guessResult.isLetterCountOk || _lastTry)
        {
            QCheckBox* checkBox = dynamic_cast<QCheckBox*>(_ui.isCorrectWordLayout->itemAt(itemRow)->widget());
            Q_ASSERT(checkBox);
            checkBox->setEnabled(true);
            if(guessResult.wordExists)
                checkBox->setCheckState(Qt::Checked);
            else
                checkBox->setCheckState(Qt::Unchecked);
        }

        if(_lastTry || guessResult.isWriteWord)
        {
            _currentItem = _ui.tableWidget->item(itemRow, 0);
            _currentItem->setBackgroundColor(Qt::red);
            for(int i=0;i<colCount;i++)
            {
                enterLetter(QString::fromStdString(_core->wordToGuess()).at(i).toUpper());
                moveCurrentItem(Right);
                _currentItem->setBackgroundColor(Qt::red);
            }
            
            if(_roundResult == RoundWon)
            {
                _winningRoundTimer.start(50);
                blinkNextLetter();
            }
            
            statusBar()->showMessage(tr("Appuyer sur Entree pour continuer..."));
            setGameState(GameWaintingForEnterKey);
        }
        else
        {
            newLine();
            setGameState(GameStarted);
        }
        return;
    }
    else if(_timeMode == TimeModeWaiting)
    {       
        if(_lastTry)
            _core->guessWord(_core->wordToGuess());
        
        if(_lastTry || (guessResult.isLetterCountOk && guessResult.wordExists && guessResult.isFirstLetterOk))
        {
            _currentItem = _ui.tableWidget->item(itemRow, 0);
            _currentAnsweringPosition = 0;
            _answeringTimer.start(200);
        }
        else
        {
            for(int i=0;i<colCount;i++)
            {
                QTableWidgetItem* item = _ui.tableWidget->item(itemRow,i);
                item->setBackgroundColor(Qt::white);
            }

            if(guessResult.isLetterCountOk)
            {
                QCheckBox* checkBox = dynamic_cast<QCheckBox*>(_ui.isCorrectWordLayout->itemAt(itemRow)->widget());
                Q_ASSERT(checkBox);
                checkBox->setEnabled(true);
                if(guessResult.wordExists)
                    checkBox->setCheckState(Qt::Checked);
                else
                    checkBox->setCheckState(Qt::Unchecked);
            }

            newLine();
            setGameState(GameStarted);
        }
    }
        
    return;
}

void MouthusMainWindow::moveCurrentItem(Direction direction)
{
    Q_ASSERT(_currentItem);

    const int & itemRow = _currentItem->row();
    const int & itemColumn = _currentItem->column();
    const int & colCount = _ui.tableWidget->columnCount();

    switch (direction) {
        case Left:
            _currentItem = _ui.tableWidget->item(itemRow, itemColumn!=0?itemColumn-1:itemColumn);
            break;
        case Right:
            _currentItem = _ui.tableWidget->item(itemRow, itemColumn!=colCount-1?itemColumn+1:colCount-1);
            break;
        default:
            break;
    }
    
    return;
}

void MouthusMainWindow::colorizeNextLetter()
{
    Q_ASSERT(_timeMode == TimeModeWaiting);
    Q_ASSERT(_answeringTimer.isActive());
    
    if(_currentAnsweringPosition==_ui.tableWidget->columnCount())
    {
        _answeringTimer.stop();
        
        const MouthusGuessResult & guessResult = _core->lastGuessResults();
        const int & itemRow = _currentItem->row();
        
        if(guessResult.isLetterCountOk)
        {
            QCheckBox* checkBox = dynamic_cast<QCheckBox*>(_ui.isCorrectWordLayout->itemAt(itemRow)->widget());
            Q_ASSERT(checkBox);
            checkBox->setEnabled(true);
            if(guessResult.wordExists)
                checkBox->setCheckState(Qt::Checked);
            else
                checkBox->setCheckState(Qt::Unchecked);
        }

        if(guessResult.isWriteWord || _lastTry)
        {
            if(_roundResult == RoundWon)
            {
                _winningRoundTimer.start(50);
                blinkNextLetter();
            }
            statusBar()->showMessage(tr("Appuyer sur Entree pour continuer..."));
            setGameState(GameWaintingForEnterKey);
        }
        else
        {
            newLine();
            setGameState(GameStarted);
        }


        return;
    }
    
    if(_lastTry)
    {
        enterLetter(QString::fromStdString(_core->wordToGuess()).at(_currentAnsweringPosition).toUpper());
        moveCurrentItem(Right);
    }
    
    colorizeLetter(_currentAnsweringPosition);
    
    _currentAnsweringPosition++;
}


void MouthusMainWindow::blinkNextLetter()
{
    Q_ASSERT(_winningRoundTimer.isActive());
    Q_ASSERT(_roundResult == RoundWon);

    const int & itemColumn = _currentItem->column();
    const int & colCount = _ui.tableWidget->columnCount();

    Q_ASSERT(itemColumn<colCount);
    
    if(_currentItem->backgroundColor() == Qt::red)
    {
        _currentItem->setBackgroundColor(Qt::white);
    }
    else
    {
        colorizeLetter(_currentItem->column());
        if(itemColumn!=colCount-1)
        {
            moveCurrentItem(Right);
        }
        else
        {
            colorizeLetter(_currentItem->column());
            while (_currentItem->column() != 0) {
                moveCurrentItem(Left);
            }
        }
    }
    
    return;
}

void MouthusMainWindow::colorizeLetter(int itemCol)
{
    const int & itemRow = _currentItem->row();

    Q_ASSERT(itemRow<_ui.tableWidget->rowCount());
    Q_ASSERT(itemCol<_ui.tableWidget->columnCount());
    
    const MouthusGuessResult & guessResult = _core->lastGuessResults();

    QTableWidgetItem* item = _ui.tableWidget->item(itemRow,itemCol);
    switch (guessResult.letterResults[itemCol]) {
        case Mouthus::LetterInPlace:
            _guessedPosition.insert(itemCol);
            item->setBackgroundColor(Qt::red);
            break;
            
        case Mouthus::LetterPresent:
            item->setBackground(QPixmap(":/images/ressources/mouthus_letter.png").scaled(30,30));
            break;
            
        case Mouthus::LetterNotPresent:
            item->setBackgroundColor(Qt::white);
            break;
            
        default:
            break;
    }

}

void MouthusMainWindow::newLine()
{
    if(!_currentItem)
    {
        _currentItem = _ui.tableWidget->item(0, 0);
    }
    else
    {
        Q_ASSERT(_currentItem->row()!=_ui.tableWidget->rowCount()-1);
        _currentItem = _ui.tableWidget->item(_currentItem->row()+1, 0);        
    }

    const int & colCount = _ui.tableWidget->columnCount();
    const int & itemRow = _currentItem->row();

    QString letter = QString::fromStdString(_core->wordToGuess()).at(0).toUpper();
    _currentItem->setData(Qt::DisplayRole,letter);
    
    for(int i=0;i<colCount;i++)
    {
        QTableWidgetItem* item = _ui.tableWidget->item(itemRow, i);
        item->setData(Qt::DisplayRole,".");
    }

    for(int i=0;i<colCount;i++)
    {
        QTableWidgetItem* item = _ui.tableWidget->item(itemRow, i);
        if(_guessedPosition.contains(i))
        {
            QString letter = QString::fromStdString(_core->wordToGuess()).at(i).toUpper();
            item->setData(Qt::DisplayRole,letter);
            item->setBackgroundColor(Qt::red);
        }
    }
}

void MouthusMainWindow::setGameState(GameState gameState)
{
    _gameState = gameState;
}

MouthusMainWindow::GameState MouthusMainWindow::gameState() const
{
    return _gameState;
}

void MouthusMainWindow::setTimeMode(TimeMode timeMode)
{
    _timeMode = timeMode;
}

MouthusMainWindow::TimeMode MouthusMainWindow::timeMode() const
{
    return _timeMode;
}


void MouthusMainWindow::setTimeModeWaiting()
{
    setTimeMode(TimeModeWaiting);
}

void MouthusMainWindow::setTimeModeNotWaiting()
{
    setTimeMode(TimeModeNotWaiting);
}


QString MouthusMainWindow::wordAtRow(int row)
{
    Q_ASSERT(row<_ui.tableWidget->rowCount());
    
    const int & colCount = _ui.tableWidget->columnCount();

    QString word;
    
    for(int col=0;col<colCount;col++)
        word.append(_ui.tableWidget->item(row, col)->text());
    
    Q_ASSERT(word.size()==colCount);
    return word;
}
