#ifndef MOUTHUS_CORE_H
#define MOUTHUS_CORE_H

#include <string>
#include <vector>

class MouthusDict;

namespace Mouthus {
        
    enum LetterResult{
        LetterNotPresent,
        LetterPresent,
        LetterInPlace
    };
}

struct MouthusGuessResult {
    bool isFirstLetterOk;
    bool isLetterCountOk;
    bool wordExists;
    std::vector<Mouthus::LetterResult> letterResults;
    bool isWriteWord;
};

class MouthusCore
{
public:
    
    MouthusCore();
    ~MouthusCore();
    
    void newWordToGuess();
    
    int tryCount() const;
    
    int letterCountMin() const;
    int letterCountMax() const;
    
    void setLetterCount(size_t letterCount);
    size_t letterCount() const;
    
    const std::string & wordToGuess() const;
    
    const MouthusGuessResult & guessWord(std::string guessWord);
    
    const MouthusGuessResult & lastGuessResults() const;
    const std::string lastGuessResultsString() const;
    
    const bool & lastWordHasBeenFound() const;
    
private:
    size_t _letterCount;
    std::string _wordToGuess;
    bool _lastWordHasBeenFound;
    MouthusGuessResult _lastGuessResults;
    MouthusDict* _dict;
    int _tryCount;
};

#endif //MOUTHUS_CORE_H