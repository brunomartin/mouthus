#include "mouthus_dict.h"

#include "mouthus_dict_ressource.h"

#include <ctime>

#include <cassert>

MouthusDict::MouthusDict()
{
    srand(time(NULL));
}

int MouthusDict::letterCountMin() const
{
    return 5;
}

int MouthusDict::letterCountMax() const
{
    return 13;
}

std::string MouthusDict::pickWord(int wordLength) const
{    
    int randomNumber;
    
    std::string word;
    switch (wordLength) {
        case 5:
            randomNumber = rand() % MouthusDictRessource::wordList5LettersSize;
            word = MouthusDictRessource::wordList5Letters[randomNumber];
            break;
        case 6:
            randomNumber = rand() % MouthusDictRessource::wordList6LettersSize;
            word = MouthusDictRessource::wordList6Letters[randomNumber];
            break;
        case 7:
            randomNumber = rand() % MouthusDictRessource::wordList7LettersSize;
            word = MouthusDictRessource::wordList7Letters[randomNumber];
            break;
        case 8:
            randomNumber = rand() % MouthusDictRessource::wordList8LettersSize;
            word = MouthusDictRessource::wordList8Letters[randomNumber];
            break;
        case 9:
            randomNumber = rand() % MouthusDictRessource::wordList9LettersSize;
            word = MouthusDictRessource::wordList9Letters[randomNumber];
            break;
        case 10:
            randomNumber = rand() % MouthusDictRessource::wordList10LettersSize;
            word = MouthusDictRessource::wordList10Letters[randomNumber];
            break;
        case 11:
            randomNumber = rand() % MouthusDictRessource::wordList11LettersSize;
            word = MouthusDictRessource::wordList11Letters[randomNumber];
            break;
        case 12:
            randomNumber = rand() % MouthusDictRessource::wordList12LettersSize;
            word = MouthusDictRessource::wordList12Letters[randomNumber];
            break;
        case 13:
            randomNumber = rand() % MouthusDictRessource::wordList13LettersSize;
            word = MouthusDictRessource::wordList13Letters[randomNumber];
            break;
        default:
            assert(0);
            break;
    }
    
    return word;
}

bool MouthusDict::verifyWordExistence(std::string word) const
{
    const char** wordList = 0;
    int wordCount = 0;

    const int & wordLength = word.length();
    
    switch (wordLength) {
        case 5:
            wordCount = MouthusDictRessource::wordList5LettersSize;
            wordList = MouthusDictRessource::wordList5Letters;
            break;
        case 6:
            wordCount = MouthusDictRessource::wordList6LettersSize;
            wordList = MouthusDictRessource::wordList6Letters;
            break;
        case 7:
            wordCount = MouthusDictRessource::wordList7LettersSize;
            wordList = MouthusDictRessource::wordList7Letters;
            break;
        case 8:
            wordCount = MouthusDictRessource::wordList8LettersSize;
            wordList = MouthusDictRessource::wordList8Letters;
            break;
        case 9:
            wordCount = MouthusDictRessource::wordList9LettersSize;
            wordList = MouthusDictRessource::wordList9Letters;
            break;
        case 10:
            wordCount = MouthusDictRessource::wordList10LettersSize;
            wordList = MouthusDictRessource::wordList10Letters;
            break;
        case 11:
            wordCount = MouthusDictRessource::wordList11LettersSize;
            wordList = MouthusDictRessource::wordList11Letters;
            break;
        case 12:
            wordCount = MouthusDictRessource::wordList12LettersSize;
            wordList = MouthusDictRessource::wordList12Letters;
            break;
        case 13:
            wordCount = MouthusDictRessource::wordList13LettersSize;
            wordList = MouthusDictRessource::wordList13Letters;
            break;
        default:
            return false;
            break;
    }
    
    for(int i=0;i<wordCount;i++)
        if(!word.compare(wordList[i]))
            return true;
    
    return false;
}
