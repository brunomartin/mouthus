#ifndef MOUTHUS_MAIN_WINDOW_H
#define MOUTHUS_MAIN_WINDOW_H

#include <QtGui>

#include "ui_mouthus_main_window.h"

class MouthusCore;
class MouthusNetworkManager;

class MouthusMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    
    enum Direction {
        Left,
        Right
    };
    
    enum GameState {
        GameStopped,
        GameStarted,
        GamePaused,
        GameWaintingForEnterKey,
        GameGivingAnswers
    };
    
    enum RoundResult {
        RoundWon,
        RoundLost
    };
    
    enum TimeMode {
        TimeModeNotWaiting,
        TimeModeWaiting
    };
    
    MouthusMainWindow(MouthusCore* core);
    ~MouthusMainWindow();
    
    GameState gameState() const;
    
    TimeMode timeMode() const;
    
    void setTryCount(int tryCount);
    
    QString wordAtRow(int row);
    
protected:
    virtual void keyPressEvent(QKeyEvent* event);
    
private:
    MouthusCore* _core;
    Ui_MainWindow _ui;
    int _tryCount;

    QTableWidgetItem* _currentItem;
    QSet<int> _guessedPosition;
    GameState _gameState;
    TimeMode _timeMode;
    
    int _currentAnsweringPosition;
    bool _lastTry;
    
    QTimer _answeringTimer;
    
    RoundResult _roundResult;
    
    QTimer _winningRoundTimer;
    int _blinkingLetter;
    
    MouthusNetworkManager* _networkManager;
    
signals:
    
public slots:
    void newRound();
    void setLetterCount(int letterCount);
    
    void setGameState(GameState gameState);
    
    void startGame();
    void stopGame();
    
    void setTimeMode(TimeMode timeMode);
    void setTimeModeWaiting();
    void setTimeModeNotWaiting();
    
    void newLine();
    
    void enterLetter(const QString & letter);
    void moveCurrentItem(Direction direction);
    void validateWord();
    
private slots:
    void colorizeLetter(int i);
    void colorizeNextLetter();
    void blinkNextLetter();
};

#endif //MOUTHUS_MAIN_WINDOW_H