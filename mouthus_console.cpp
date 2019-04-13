#include "mouthus_core.h"

#include <iostream>

int main(int argc, char* argv[])
{
    MouthusCore core;
        
    size_t guessWordLength;
   
    while (1) {
        std::cout << "rentrez le nombre de lettre (5-10) : " << std::endl;
        std::cin >> guessWordLength;
        
        if(guessWordLength >= 5 && guessWordLength <= 10)
        {
            core.setLetterCount(guessWordLength);
            core.newWordToGuess();
            break;
        }
    }

    std::string guessWord;
    
    while (!core.lastGuessResults().isWriteWord) {
        std::cout << "devinez un mot de " << core.letterCount() <<  " lettres (" << core.wordToGuess() << "), essai n°" << core.tryCount()+1 << " :" << std::endl;
        std::cin >> guessWord;
        
        MouthusGuessResult guessResult = core.guessWord(guessWord);
        
        if(!guessResult.isLetterCountOk)
        {
            std::cout << "Vous devez rentrer un mot de " << core.letterCount() << " lettres" << std::endl;
            continue;
        }
        else if(!guessResult.wordExists)
        {
            std::cout << "Le mot \"" << guessWord << "\" n'existe pas" << std::endl;
            continue;
        }
        
        std::cout << core.lastGuessResultsString() << std::endl;
    }
    
    std::cout << "Bravo ! vous avez deviné le mot \"" << core.wordToGuess() << "\"" << std::endl;
}