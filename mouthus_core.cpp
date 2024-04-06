#include "mouthus_core.h"

#include "mouthus_dict.h"

#include <cassert>
#include <iostream>

MouthusCore::MouthusCore() : _letterCount(0), _wordToGuess(""), _lastWordHasBeenFound(false), _dict(new MouthusDict), _tryCount(0)
{
    _lastGuessResults.isFirstLetterOk = false;
    _lastGuessResults.isLetterCountOk = false;
    _lastGuessResults.wordExists = false;
    _lastGuessResults.letterResults = std::vector<Mouthus::LetterResult>();
    _lastGuessResults.isWriteWord = false;
}

MouthusCore::~MouthusCore()
{
}

void MouthusCore::setLetterCount(size_t letterCount)
{
    assert(letterCount>=_dict->letterCountMin());
    assert(letterCount<=_dict->letterCountMax());
    
    _letterCount = letterCount;
}

size_t MouthusCore::letterCount() const
{
    return _letterCount;
}

void MouthusCore::newWordToGuess()
{
    _tryCount = 0;
    assert(_letterCount);
    _wordToGuess = _dict->pickWord(_letterCount);    
}

const std::string & MouthusCore::wordToGuess() const
{
    return _wordToGuess;
}

int MouthusCore::tryCount() const
{
    return _tryCount;
}

const MouthusGuessResult & MouthusCore::guessWord(std::string guessWord)
{
    assert(_wordToGuess[0] != '%');
    
    _tryCount++;
    
    std::vector<Mouthus::LetterResult> letterResults(_wordToGuess.length());
    
    std::string wordToGuess = _wordToGuess;
    
    _lastGuessResults.isFirstLetterOk = false;
    _lastGuessResults.isLetterCountOk = false;
    _lastGuessResults.wordExists = false;
    _lastGuessResults.letterResults = std::vector<Mouthus::LetterResult>();
    _lastGuessResults.isWriteWord = false;
        
    if(guessWord.length() == _wordToGuess.length())
        _lastGuessResults.isLetterCountOk = true;

    if(guessWord.length())
        if(guessWord.at(0) == _wordToGuess.at(0))
            _lastGuessResults.isFirstLetterOk = true;

    if(_lastGuessResults.isLetterCountOk)
        _lastGuessResults.wordExists = _dict->verifyWordExistence(guessWord);

    if(!(_lastGuessResults.isFirstLetterOk && _lastGuessResults.isLetterCountOk))
        return _lastGuessResults;
    
    if(!_lastGuessResults.wordExists)
        return _lastGuessResults;

    if(!(_lastGuessResults.isFirstLetterOk
         && _lastGuessResults.isLetterCountOk
         &&_lastGuessResults.wordExists))
        return _lastGuessResults;

    _lastGuessResults.isWriteWord = true;
    
    for(size_t i=0;i<guessWord.length();i++)
    {
        if(guessWord[i] == wordToGuess[i])
        {
            letterResults[i] = Mouthus::LetterInPlace;
			wordToGuess.replace(i,1,"%");
            guessWord.replace(i,1,"%");
        }
    }
    
    for(size_t i=0;i<guessWord.length();i++)
    {
        if(guessWord[i] == '%')
        {
			continue;
        }
        else if(wordToGuess.find_first_of(guessWord[i]) != std::string::npos)
        {
            letterResults[i] = Mouthus::LetterPresent;
            wordToGuess.replace(wordToGuess.find_first_of(guessWord[i]),1,"%");
            _lastGuessResults.isWriteWord = false;
        }
        else
        {
            letterResults[i] = Mouthus::LetterNotPresent;
            _lastGuessResults.isWriteWord = false;
        }
    }

    _lastGuessResults.letterResults = letterResults;

    return _lastGuessResults;
}


const bool & MouthusCore::lastWordHasBeenFound() const
{
    return _lastWordHasBeenFound;
}

const MouthusGuessResult & MouthusCore::lastGuessResults() const
{
    return _lastGuessResults;
}

const std::string MouthusCore::lastGuessResultsString() const
{
    std::string resultString(_wordToGuess.length(),'%');
    
    for(size_t i=0;i<_wordToGuess.length();i++)
    {
        switch (_lastGuessResults.letterResults[i]) {
            case Mouthus::LetterNotPresent:
                resultString[i] = '0';
                break;
            case Mouthus::LetterPresent:
                resultString[i] = '1';
                break;
            case Mouthus::LetterInPlace:
                resultString[i] = '2';
                break;                
            default:
                break;
        }
    }
    
    return resultString;
}

int MouthusCore::letterCountMin() const
{
    return _dict->letterCountMin();
}

int MouthusCore::letterCountMax() const
{
    return _dict->letterCountMax();
}

